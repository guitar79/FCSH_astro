﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ASCOM.TFocuser
{
    public class FocuserMotorChangedEventArgs : EventArgs 
    {
        public readonly int Motor;

        public FocuserMotorChangedEventArgs(int motor)
        {
            this.Motor = motor;
        }
    }
}
