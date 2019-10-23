using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ASCOM.GSfocus
{
    public class FocuserTemperatureChangedEventArgs : EventArgs
    {
        public readonly string Temperature;

        public FocuserTemperatureChangedEventArgs(string temperature)
        {
            this.Temperature = temperature;
        }
    }
}
