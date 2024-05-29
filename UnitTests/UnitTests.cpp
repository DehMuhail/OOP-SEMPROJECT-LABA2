#include "pch.h"
#include "CppUnitTest.h"
#include "..\tc\raycaster.h"
#include "..\tc\RandomMove.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTests
{

	GameState gs =
	{ Map(1, 16 ,16),                         // game map
  {3.456, 2.345, 1.523, M_PI / 3., 0, 0, 15}, // player
  { {3.523, 3.812, 2, 0, 3},				  // monsters lists
		//{1.834, 8.765, 0, 0, 3},
		//{5.323, 5.365, 1, 0, 3},
		//{14.32, 13.36, 3, 0, 3},
		//{4.123, 10.76, 1, 0, 3} 
		  },
	  TextureFactory::createTexture("walltext.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
	  TextureFactory::createTexture("monsters.bmp", SDL_PIXELFORMAT_ABGR8888), // textures for the monsters
	  TextureFactory::createTexture("gun.bmp", SDL_PIXELFORMAT_ABGR8888) };

	TEST_CLASS(UnitTests)
	{
	public:	
		TEST_METHOD(TestMethod1)
		{
			//Assert::IsTrue(1 == 1);
			
			for (int i = 0; i < 1000000; i++)
			{
				RMMbody(gs.monsters[0], gs.map);
				Assert::IsTrue(int(gs.monsters[0].x) >= 0 && int(gs.monsters[0].x) < int(gs.map.w) 
							&& int(gs.monsters[0].y) >= 0 && int(gs.monsters[0].y) < int(gs.map.h));
				Assert::IsTrue(gs.map.is_empty(gs.monsters[0].x, gs.monsters[0].y));
			}
		}


		TEST_METHOD(TestMethod2)
		{
			int TestSize = gs.monsters.size();
			int testHP = gs.monsters[0].health;
			
			for (int i = 0; i < 100000; i++)
			{
				gs.player.a += (M_PI / 180) * 0.01;
				gs.player.shoot(gs.monsters, gs.map);
				
			}
			Assert::IsTrue(gs.monsters.size() < TestSize || gs.monsters[0].health < testHP);
		}

		TEST_METHOD(TestMethod3)
		{
			GameState gs =
			{ Map(1, 16 ,16),
			{3.456, 2.345, 1.523, M_PI / 3., 0, 0, 15},
			{ {14.32, 13.36, 3, 0, 3},},
			  TextureFactory::createTexture("walltext.bmp", SDL_PIXELFORMAT_ABGR8888),
			  TextureFactory::createTexture("monsters.bmp", SDL_PIXELFORMAT_ABGR8888),
			  TextureFactory::createTexture("gun.bmp", SDL_PIXELFORMAT_ABGR8888) };		
			int TestSize = gs.monsters.size();
			int testHP = gs.monsters[0].health;

			for (int i = 0; i < 100000; i++)
			{
				gs.player.a += (M_PI / 180) * 0.01;
				gs.player.shoot(gs.monsters, gs.map);

			}
			Assert::IsTrue(gs.monsters.size() == TestSize || gs.monsters[0].health == testHP);
		}
	};
}
