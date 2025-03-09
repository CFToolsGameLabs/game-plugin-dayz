class CFCloud_WorldWeather extends GameLabsContextAction {
        void CFCloud_WorldWeather() {
            this.actionCode = "CFCloud_WorldWeather";
            this.actionName = "Update weather";
            this.actionIcon = "clouds-sun";
            this.actionColour = "default";
            this.actionContext = "world";

            this.parameters.Insert("overcast", GameLabsActionParameter("Overcast", "Number between 0 and 1", "float"));
            this.parameters.Insert("fog", GameLabsActionParameter("Fog", "Number between 0 and 1", "float"));
            this.parameters.Insert("rain", GameLabsActionParameter("Rain", "Number between 0 and 1", "float"));
            this.parameters.Insert("snowfall", GameLabsActionParameter("Snowfall", "Number between 0 and 1", "float"));
            this.parameters.Insert("wind", GameLabsActionParameter("Wind speed", "Wind speed in km/h", "int"));
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather overcast=%1, fog=%2, rain=%3, wind=%4",context.parameters.Get("overcast").GetFloat(),context.parameters.Get("fog").GetFloat(),context.parameters.Get("rain").GetFloat(),context.parameters.Get("wind").GetInt()));

            Weather weather = GetGame().GetWeather();
            if(!weather) return false;

            if(weather.GetOvercast()) weather.GetOvercast().Set(context.parameters.Get("overcast").GetFloat(), context.parameters.Get("overcast").GetFloat(), context.parameters.Get("overcast").GetFloat());
            if(weather.GetFog()) weather.GetFog().Set(context.parameters.Get("fog").GetFloat(), context.parameters.Get("fog").GetFloat(), context.parameters.Get("fog").GetFloat());
            if(weather.GetRain()) weather.GetRain().Set(context.parameters.Get("rain").GetFloat(), context.parameters.Get("rain").GetFloat(), context.parameters.Get("rain").GetFloat());
            if(weather.GetSnowfall())  weather.GetRain().Set(context.parameters.Get("snowfall").GetFloat(), context.parameters.Get("snowfall").GetFloat(), context.parameters.Get("snowfall").GetFloat());
            weather.SetWindSpeed(context.parameters.Get("wind").GetInt());

            return true;
        }
};