using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ASCOM.GSfocus
{
    public class FocuserHumidityChangedEventArgs : EventArgs
    {
        public readonly string Humidity;

        public FocuserHumidityChangedEventArgs(string humidity)
        {
            this.Humidity = humidity;
        }
    }
}
