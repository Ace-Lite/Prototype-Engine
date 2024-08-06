using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sprite_Model_Editor.InOut
{
    internal class Conversions
    {
        // DOOM FRACUNIT -- Fixed Point Math
        const int FRACUNIT = 65536;

        public float Math_FracToFloat(int unit)
        {
            return (float)(unit / FRACUNIT);
        }

        public int Math_FloatToFrac(float unit)
        {
            return (int)(unit * FRACUNIT);
        }

        public int Math_FracToInt(int unit)
        {
            return (unit / FRACUNIT);
        }

        public int Math_IntToFrac(int unit)
        {
            return (unit * FRACUNIT);
        }

    }
}
