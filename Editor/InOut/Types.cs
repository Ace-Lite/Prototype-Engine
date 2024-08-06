using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sprite_Model_Editor.InOut
{
    // SRB2 has 3 types of sprites (In outside uses, I want to use merely Camera Sprites and Plane Sprites)
    // Camera Sprites - Sprites facing Camera,
    // Paper Sprites - Sprites acting as walls -- exclusive to SRB2 and GZDoom
    // Plane Sprites - Sprites with full rotation
    enum spritetype_e
    {
        CameraSprite,
        PaperSprite,
        PlaneSprite,
    }

    enum modelformat_e
    {
        Doom, // For SRB2 purposes
        Modern, // Converts paper sprites into model geometry.
    }

    // Most use cases don't use floats, they are fixed point.
    // Therefore Int type vectors are required

    struct intvec2_t
    {
        public int x, y;
    }

    struct intvec3_t
    {
        public int x, y, z;
    }

    // Struct for image data

    struct imagedata_t
    {
        //Image data
        string name;
    }

    // Invidiual object data rendered on screen

    struct animationdata_t
    {
        int frame;
        bool interpolation;
        public intvec3_t newpos;
        public intvec3_t newrot;
        imagedata_t change;
    }

    struct spritedata_t
    {
        // Layer data
        public int id;
        public string name;
        public bool visible;
        public int jointed;

        // Render info
        public spritetype_e type;
        public animationdata_t[] anim;
        public imagedata_t img;
        public intvec3_t pos;
        public intvec3_t rot;
    }
    struct modeldata_t
    {
        public modelformat_e m;
        public spritedata_t[] data;
    }
}

//
//  Format Header
//
//  modeldata_t -> Binary format -> ".SPRMO"
//
//  
//