/* --------------------------- */
/* Inside the machine 2        */
/* --------------------------- */

#ifndef H_FXHIT
#define H_FXHIT

class WorldManagerOffline;
namespace LowLevel
{
	class Sprite;
	class fxHit
	{
		public:
			fxHit();
			void Draw();
			void Update();
			void Load();
			void Reset();
			void Set(int x, int y, int w, int h, int time);

		public:
			Sprite* img;
			WorldManagerOffline* worldMan;

			int x, y, w, h;
			int type;
			int alpha;
			int time;
			int pg;
			bool toDelete;
			bool free;

	};
};

#endif