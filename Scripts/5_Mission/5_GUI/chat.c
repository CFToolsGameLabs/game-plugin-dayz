modded class Chat {
    const string MESSAGE_HANDLER_PREFIX = "|>";

    void AddGameLabs(ChatMessageEventParams params) {
        int channel = params.param1;
        string custom_colour;

        if(channel & CCBattlEye) {
            string message = params.param3;

            int prefix_position = message.IndexOf(MESSAGE_HANDLER_PREFIX);
            if (prefix_position != -1) {
                string messageType = message.Get(prefix_position + MESSAGE_HANDLER_PREFIX.Length());
                if (messageType == "C") { // Coloured
                    custom_colour = message.Substring(prefix_position + MESSAGE_HANDLER_PREFIX.Length() + 1, 6);
                    message = message.Substring(prefix_position + MESSAGE_HANDLER_PREFIX.Length() + 6 + 1, message.Length() - (prefix_position + MESSAGE_HANDLER_PREFIX.Length() + 6 + 1));
                    params.param3 = message;
                } else {
                    return; // Invalid Message Encoding
                }
            }
        }

        m_LastLine = (m_LastLine + 1) % m_Lines.Count();

        ChatLine line = m_Lines.Get(m_LastLine);
        line.Set(params);

        if(channel & CCBattlEye && custom_colour) {
            int colour = GameLabsUtil.hex_to_rgb(custom_colour);
            line.m_NameWidget.SetColor(colour);
            line.m_TextWidget.SetColor(colour);
        }

        for (int i = 0; i < m_Lines.Count(); i++) {
            line = m_Lines.Get((m_LastLine + 1 + i) % LINE_COUNT);
            line.m_RootWidget.SetPos(0, i * m_LineHeight);

            float x = 0;
            float y = 0;

            line.m_RootWidget.GetPos(x, y);
        }
    }
};