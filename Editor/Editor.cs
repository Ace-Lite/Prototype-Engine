using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sprite_Model_Editor
{
    public partial class Editor : Form
    {
        Graphics[] steps = new Graphics[32];

        SolidBrush brush;
        Graphics plane;

        int step_position = 0;

        public Editor()
        {
            InitializeComponent();
            brush = new SolidBrush(Color.Black);
            plane = panel1.CreateGraphics();
        }

        private void panel1_MouseClick(object sender, MouseEventArgs e)
        {            
            if (step_position > 0)
            {
                steps = steps.Take(steps.Count() - step_position).ToArray();
                step_position = 0;
            }

            plane.FillRectangle(brush, e.Location.X, e.Location.Y, 128, 128);

            if (steps.Count() > 31)
                steps = steps.Skip(1).ToArray();

            steps = steps.Append(plane).ToArray();
        }

        private void toolStripButton5_Click(object sender, MouseEventArgs e)
        {
            if (step_position > 30) return;

            step_position = Math.Min(step_position + 1, steps.Count());

            plane = steps[steps.Count() - step_position];
        }
    }
}
