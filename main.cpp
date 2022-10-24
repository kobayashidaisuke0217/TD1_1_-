#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
const char kWindowTitle[] = "TD1_回レ回レ";

typedef struct Vector2 {
	float x;
	float y;
};

typedef struct Player {
	Vector2 pos;
	Vector2 vector;
	float R;
	float speed;
	float radian;
	int alive;
	int hit;
};

typedef struct Atack
{
	Vector2 pos;
	Vector2 old;
	float speed;
	int flag;
	float R;
};

typedef struct Enemy {
	Vector2 pos;
	Vector2 start;
	float speed;
	int alive;
	int count;
	int arrival;
	float R;
	int spawnCount;
	Atack atack;
	unsigned int color;
	int EndFlag;
	Vector2 rand;
	int randCount;
	int hitFlag;
	
};
void RandRespown(Vector2 &start ) {
	int num = rand()  ;
	int randSpawn = num % 6+1;
	if (randSpawn == 1) {
		start.x = -30;
		start.y = -100;
		
	}
	else if (randSpawn == 2) {
		start.x = -30;
		start.y = 200;
		
	}
	else if (randSpawn == 3) {
		start.x = -30;
		start.y = 700;
	}
	else if (randSpawn == 4) {
		start.x = 1300;
		start.y = -100;
	}
	else if (randSpawn == 5) {
		start.x = 1300;
		start.y = 200;

	}
	else if (randSpawn == 6) {
		start.x = 1300;
		start.y = 700;
	}
	
}
void randShake(Vector2& randam,int &count,int flag) {
	 count--;
	 
	 if (count > 0) {
		 randam.x = rand() % count;
		 randam.y = rand() % count;
		 randam.x = rand() % count - count / 2;
		 randam.y = rand() % count - count / 2;
	 }
	 
}

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// srand 
	srand(time(nullptr));


	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player;
	player.R = 20;

	player.pos.x = 640;
	player.pos.y = 360;
	player.vector.x = 200;
	player.vector.y = 200;
	player.speed = 5;
	player.radian = 1.0f / 128.0f * M_PI;
	player.alive = true;
	player.hit = false;
	float p2eX = 0;
	float p2eY = 0;

	Enemy enemy[100];
	for (int i = 0; i < 100; i++) {
		enemy[i].pos.x = 0;
		enemy[i].pos.y = 0;
		enemy[i].alive = false;
		enemy[i].arrival = false;
		enemy[i].R = 32;
		enemy[i].speed = 0;
		enemy[i].start.x = 0;
		enemy[i].start.y = 0;
		enemy[i].atack.flag = false;
		enemy[i].atack.pos.x = 0;
		enemy[i].atack.pos.y = 0;
		enemy[i].atack.speed = 0;
		enemy[i].atack.old.x = 0;
		enemy[i].atack.old.y = 0;
		enemy[i].atack.R = 32;
		enemy[i].color = WHITE;
		enemy[i].spawnCount = 0;
		enemy[i].EndFlag = false;
		enemy[i].hitFlag = false;
	}
	float theat = 0.0f;
	
	enum {
		flagtrue,
		flagfalse,
		flagnum,

	};
	//反転
	int reversal = false;

	float player2R = 285;
	float c = 0.0f;
	float d = 0.0f;
	float distancePlayer = 0.0f;
	float distanceEnemy = 0.0f;
	enum {
		title,
		game,
		result,
	};
	int Scene = title;
	Vector2 rand;
	 rand.x = 0;
	 rand.y = 0;
	int randCount = 100;
	Vector2 rotatedVector={0,0};
	Vector2 player2={0,0};
	int SceneCircleSize = 20;
	int SceneFlag = false;
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		switch (Scene)
		{
		case title:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				SceneFlag = true;
				
			}
			if (SceneFlag == true) {
				SceneCircleSize += 5;
			}
			if (SceneCircleSize >= 640) {
				Scene = game;
			}
			break;
		case game:
			if (player.hit == false) {
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && reversal == false) {
					reversal = true;
				}
				else if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && reversal == true) {
					reversal = false;
				}

				if (reversal == false) {
					theat += player.radian;
				}
				if (reversal) {
					theat -= player.radian;
				}
			}

			/*--------
			　回転
			---------*/
			rotatedVector.x = player.vector.x * cos(theat) - player.vector.y * sin(theat);
			rotatedVector.y= player.vector.x* sin(theat) + player.vector.y * cos(theat);
			
			player2.x = player.pos.x + rotatedVector.x;
			player2.y = player.pos.y + rotatedVector.y ;
			enemy[0].alive = true;
			enemy[0].pos.x = player.pos.x;
			enemy[0].pos.y = player.pos.y;
			enemy[0].speed = 1;
			for (int i = 0; i < 100; i++) {
				c = player.pos.x - enemy[i].pos.x;
				d = player.pos.y - enemy[i].pos.y;
				distanceEnemy = sqrt(c * c + d * d);

				if (player.hit == false) {
					enemy[i].spawnCount++;
				}
				if (enemy[i].spawnCount == i * 120) {
					enemy[i].alive = true;
					RandRespown(enemy[i].start);
				}
				if (enemy[i].alive == true) {
					if (i != 0) {
						if (player.hit == false) {
							if (enemy[i].speed < 1) {
								enemy[i].speed += 0.005;
							}
							enemy[i].pos.x = (1.0f - enemy[i].speed) * enemy[i].start.x + enemy[i].speed * player.pos.x;
							enemy[i].pos.y = (1.0f - enemy[i].speed) * enemy[i].start.y + enemy[i].speed * player.pos.y;
						}
					}
					}
				

				if (enemy[i].speed >= 1 && enemy[i].atack.flag == false) {
					enemy[i].count++;

					

				}

				if (enemy[i].count % 30 == 0 && enemy[i].atack.flag == false) {
					enemy[i].atack.flag = true;
					enemy[i].atack.pos.x = enemy[i].pos.x;
					enemy[i].atack.pos.y = enemy[i].pos.y;
					enemy[i].atack.speed = 0;
					enemy[i].atack.old.x = player2.x;
					enemy[i].atack.old.y = player2.y;
				}
				if (enemy[i].atack.flag == true) {


					if (player.hit == false) {
						if (enemy[i].atack.speed <= 1) {
							enemy[i].atack.speed += 0.03;
						}
						else {
							enemy[i].atack.speed += 0.01;
						}

						enemy[i].atack.pos.x = (1.0f - enemy[i].atack.speed) * enemy[i].pos.x + enemy[i].atack.speed * enemy[i].atack.old.x;
						enemy[i].atack.pos.y = (1.0f - enemy[i].atack.speed) * enemy[i].pos.y + enemy[i].atack.speed * enemy[i].atack.old.y;

						if (enemy[i].atack.pos.x <= player2.x + player.R && player2.x <= enemy[i].atack.pos.x + enemy[i].atack.R &&
							enemy[i].atack.pos.y <= player2.y + player.R && player2.y <= enemy[i].atack.pos.y + enemy[i].atack.R) {

							player.hit = true;
						}
					}
					if (player.hit == true) {
						randShake(rand, randCount, player.alive);
					}
					if (randCount <= 0) {
						player.alive = false;
					}




					for (int j = 0; j < 100; j++) {

						if (enemy[j].EndFlag == false) {
							if (enemy[i].atack.pos.x <= enemy[j].pos.x + enemy[i].R && enemy[j].pos.x <= enemy[i].atack.pos.x + enemy[j].atack.R &&
								enemy[i].atack.pos.y <= enemy[j].pos.y + enemy[i].R && enemy[j].pos.y <= enemy[i].atack.pos.y + enemy[j].atack.R) {
								enemy[j].alive = false;



							}
						}

					}



					//	画面端で消える
					if (enemy[i].atack.pos.x >= 1300 || enemy[i].atack.pos.x <= -20) {
						enemy[i].atack.flag = false;
					}
					if (enemy[i].atack.pos.y >= 800 || enemy[i].atack.pos.y <= -20) {
						enemy[i].atack.flag = false;
					}
				}


				if (distanceEnemy <= player2R - 64 + enemy[i].R) {
					enemy[i].color = BLUE;
					enemy[i].EndFlag = true;


				}
				//敵とプレイやーの当たり判定
				p2eX = player2.x - enemy[i].pos.x;
				p2eY = player2.y - enemy[i].pos.y;
				float p2eR = player.R + enemy[i].R;
				if (enemy[i].alive == true) {
					if (p2eX * p2eX + p2eY * p2eY <= p2eR * p2eR) {
						player.hit = true;
					}
				}

			}
			if (player.alive == false) {
				SceneFlag = false;
			}
			if (SceneFlag == false) {
				SceneCircleSize -= 5;
			}
			if (SceneCircleSize <= 0) {
				Scene = result;
			}
			break;
			case result:
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
					Scene = title;
				}
				break;
			default:
			break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		switch (Scene)
		{
		case title:
			Novice::ScreenPrintf(100, 100,"title");
			if (SceneFlag == true) {
				Novice::DrawEllipse(player.pos.x, player.pos.y, SceneCircleSize, SceneCircleSize, 0, RED, kFillModeSolid);
			}
			break;
		case game:

			Novice::DrawEllipse(enemy[0].pos.x + rand.x, enemy[0].pos.y + rand.y, enemy[0].R, enemy[0].R, 0, enemy[0].color, kFillModeSolid);
			for (int i = 0; i < 100; i++) {
				if (enemy[i].alive == true) {
					if (enemy[i].speed != 1) {
						Novice::DrawEllipse(enemy[i].pos.x + rand.x, enemy[i].pos.y + rand.y, enemy[i].R, enemy[i].R, 0, enemy[i].color, kFillModeSolid);
					}
				}
				if (enemy[i].atack.flag == true) {
					Novice::DrawEllipse(enemy[i].atack.pos.x + rand.x, enemy[i].atack.pos.y + rand.y, enemy[i].atack.R, enemy[i].atack.R, 0, RED, kFillModeSolid);
				}
			}
			if (player.alive == true) {
				Novice::DrawEllipse(
					player2.x + rand.x, player2.y + rand.y,
					player.R, player.R,
					0.0f, WHITE, kFillModeSolid);
			}
			Novice::DrawEllipse(
				player.pos.x + rand.x, player.pos.y + rand.y,
				285, 285,
				0.0f, WHITE, kFillModeWireFrame
			);
			if (SceneFlag == false) {
				Novice::DrawEllipse(player.pos.x, player.pos.y, SceneCircleSize, SceneCircleSize, 0, RED, kFillModeSolid);
			}
			break;
		case result:
			Novice::ScreenPrintf(100, 100, "result");
			break;
		default:
			break;
		}
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
