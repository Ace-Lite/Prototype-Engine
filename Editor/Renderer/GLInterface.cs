using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprite_Model_Editor.InOut;



namespace Sprite_Model_Editor.Renderer
{

    internal class Renderer
    {
        class Camera
        {
            //Vector3 Pos = new Vector3(0f, 0f, 0f);
            //Vector3 Rot = new Vector3(0f, 0f, 0f);
        }
        

        public modeldata_t Active_Object;


        private void Render_Obj()
        {
            foreach (spritedata_t data in Active_Object.data)
            {
                //Vector3 Pos = new Vector3((float)data.pos.x, (float)data.pos.y, (float)data.pos.z);
                //Vector3 Rot = new Vector3((float)data.rot.x, (float)data.rot.y, (float)data.rot.z);
            
            
            
            
            }

        }

        public void RenderScreen(intvec2_t pos, intvec2_t size)
        {
            Camera camera = new Camera();
            //Render_Obj();
        }



    }
   
}
