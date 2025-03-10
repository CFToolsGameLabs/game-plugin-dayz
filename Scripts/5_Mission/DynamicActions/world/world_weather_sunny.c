class CFCloud_WorldWeatherSunny extends GameLabsContextAction {
        void CFCloud_WorldWeatherSunny() {
            this.actionCode = "CFCloud_WorldWeatherSunny";
            this.actionName = "Set clear weather";
            this.actionIcon = "sun";
            this.actionColour = "success";
            this.actionContext = "world";
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather (sunny/clear)"));

            Weather weather = GetGame().GetWeather();
            if(!weather) return false;

            if(weather.GetOvercast()) weather.GetOvercast().Set(0.0, 0.0, 0.0);
            if(weather.GetFog()) weather.GetFog().Set(0.0, 0.0, 0.0);
            if(weather.GetRain()) weather.GetRain().Set(0.0, 0.0, 0.0);
            if(weather.GetSnowfall()) weather.GetSnowfall().Set(0.0, 0.0, 0.0);
            weather.SetWindSpeed(0);

            return true;
        }
};