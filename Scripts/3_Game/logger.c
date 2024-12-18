class GameLabsLogger {
    private static const string basePath = "$profile:@Logging";
    private string fullPath;

    private string modIdentifier;
    private FileHandle fh;

    private bool allowDebug;
    private bool logrotate;

    void GameLabsLogger(string _modIdentifier, bool _allowDebug = false, bool _logrotate = true) {
        this.allowDebug = _allowDebug;
        this.logrotate  = _logrotate;

        this.modIdentifier = _modIdentifier;

        this._Setup();
    }

    void OverrideDebugStatus(bool _allowDebug) {
        this.allowDebug = _allowDebug;
    }

    void Debug(string message) {
        if(!this.allowDebug) return;
        this.Write("DEBUG\t", message);
    }

    void Info(string message) {
        this.Write("INFO\t", message);
    }

    void Warn(string message) {
        this.Write("WARN\t", message);
    }

    void Error(string message) {
        this.Write("ERROR\t", message);
    }

    void Close() {
        delete this;
    }

    private void Write(string level, string message) {
        string logged = string.Format("%1 | [%2] %3", this.GetISO8601DT(), level, message);
        FPrintln(this.fh, logged);
        if(this.allowDebug) {
            Print(string.Format("[GameLabs-Debug] %1", logged));
        }
    }

    private string GetTimeZoneOffset() {
        int localHour, localMinute, localSecond;
        int utcHour, utcMinute, utcSecond;
        int localYear, localMonth, localDay;
        int utcYear, utcMonth, utcDay;

        // Get local time
        GetHourMinuteSecond(localHour, localMinute, localSecond);
        GetYearMonthDay(localYear, localMonth, localDay);

        // Get UTC time
        GetHourMinuteSecondUTC(utcHour, utcMinute, utcSecond);
        GetYearMonthDayUTC(utcYear, utcMonth, utcDay);

        // Calculate total seconds for local and UTC times
        int localTotalSeconds = (localHour * 3600) + (localMinute * 60) + localSecond;
        int utcTotalSeconds = (utcHour * 3600) + (utcMinute * 60) + utcSecond;

        // Handle day rollover if needed
        if(localYear != utcYear || localMonth != utcMonth || localDay != utcDay) {
            if (localYear > utcYear || (localMonth > utcMonth) || (localDay > utcDay)) {
                // Local time is ahead of UTC, add 24 hours (86400 seconds) to UTC
                utcTotalSeconds += 86400;
            }
            else {
                // UTC is ahead of local time, add 24 hours (86400 seconds) to local
                localTotalSeconds += 86400;
            }
        }

        // Compute the offset in seconds
        int offsetSeconds = localTotalSeconds - utcTotalSeconds;

        // Convert offset to hours and minutes
        int offsetHours = offsetSeconds / 3600;
        int offsetMinutes = (offsetSeconds % 3600) / 60;

        // Format as "+hh:mm" or "-hh:mm"
        return string.Format("%1%2:%3",
                             offsetHours >= 0 ? "+" : "-",
                             Math.Abs(offsetHours).ToStringLen(2),
                             Math.Abs(offsetMinutes).ToStringLen(2));
    }

    private string GetISO8601DT() {
        int s, mi, h, d, mo, y;

        GetHourMinuteSecondUTC(h, mi, s);
        GetYearMonthDayUTC(y, mo, d);

        return string.Format(
            "%1-%2-%3T%4:%5:%6+00:00",
            y.ToStringLen(2),
            mo.ToStringLen(2),
            d.ToStringLen(2),
            h.ToStringLen(2),
            mi.ToStringLen(2),
            s.ToStringLen(2),
        );
    }

    private void _Setup() {
        this.fullPath = this.basePath + "/" + this.modIdentifier + ".log";
        if(!FileExist(this.basePath)) MakeDirectory(basePath);

        if(this.logrotate && FileExist(this.fullPath)) {
            string pattern = this.fullPath + "*";
            string fileName;
            FileAttr fileAttributes;
            FindFileHandle fileSearch = FindFile(pattern, fileName, fileAttributes, FindFileFlags.ALL);

            int resultsCounter = 0;

            bool hasResults = true;
            while(hasResults) {
                resultsCounter++;
                hasResults = FindNextFile(fileSearch, fileName, fileAttributes);
            }
            CloseFindFile(fileSearch);

            string archive_name = this.fullPath + "." + resultsCounter;
            CopyFile(this.fullPath, archive_name);
            DeleteFile(this.fullPath);
        }

        this.fh = OpenFile(this.fullPath, FileMode.APPEND);

        this._WriteDTSeperator();
    }

    private void _WriteDTSeperator() {
        FPrintln(this.fh, string.Format("************************** %1 **************************", this.GetISO8601DT()));
    }
    private void _WriteClosingLine() {
        FPrintln(this.fh, string.Format("EOF @ %1", this.GetISO8601DT()));
    }
};