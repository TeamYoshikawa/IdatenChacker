#pragma once
#include <windows.h>

#pragma comment(lib,"msimg32.lib")
#include <math.h>	// atan2()を使うために必要
#include <time.h>   // time()を使うのに必要

#include "EnemyManager.h"
#include "ItemManager.h"
#include"scroll.h"
#include"Block.h"
#include"Option.h"


//chara
#include"Player_Status.h"
#include "Paint_Player.h"
#include "Player_Move.h"



#include"player_info.h"


//デバッグ
#include"debugmsg.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// ウインドウプロシージャ関数

enum Scene{ Title, Stage1, End };
int SceneNum;

int Paint(HDC);
int Paint_BG(HDC);
int Init_Game();
int Get_Key();
int SceneChanger();

#define KEY_SPACE 32

HBITMAP title_hb;	// ビットマップハンドル
HBITMAP end_hb;	// ビットマップハンドル
HBITMAP player_hb;

BYTE key_input_buff;// キーボード情報
BYTE key_buff[256];

HWND hWnd;







PAINT pl_paint_obj;

OIL *oilobj;
MOVE *pl_move_obj;
Scroll *scrobj;
Block *blobj;
EnemyManager *eobj;
ItemManager *iobj;

/////メイン関数///////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	// ウインドウクラス構造体
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;         // ウインドウスタイル
	wc.lpfnWndProc = (WNDPROC)WndProc;                // ウインドウプロシージャアドレス
	wc.cbClsExtra = 0;                               // 補助領域サイズ（使用しない）
	wc.cbWndExtra = 0;                               // 補助領域サイズ（使用しない）
	wc.hInstance = hInstance;                       // インスタンスハンドル
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // アイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);     // マウスカーソル
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);      // ウィンドウ画面の色
	wc.lpszMenuName = NULL;                            // ウインドウメニュー（使用しない）
	wc.lpszClassName = TEXT("Window01");                // ウインドウクラス名

	// ウインドウクラスの登録
	if (!RegisterClass(&wc)) return FALSE;

	// ウインドウの作成
	hWnd = CreateWindow(
		TEXT("Window01"), TEXT("STG 自機の弾"),					// ウインドウのタイトル名
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,			// ウインドウスタイル
		CW_USEDEFAULT,												// ウィンドウの表示X座標
		CW_USEDEFAULT,												// ウィンドウの表示Y座標
		WINDOW_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,						// ウィンドウの幅
		WINDOW_HEIGHT + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// 高さ
		NULL,														// 親ウインドウ
		NULL,														// ウインドウメニュー
		hInstance,													// インスタンスハンドル
		NULL);														 // WM_CREATE情報

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);                         // 表示状態の設定
	UpdateWindow(hWnd);                                 // クライアント領域の更新

	// メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// WM_USER+1へ　(ここがゲームループ)
			SendMessage(hWnd, WM_USER + 1, 0, 0);

			// スリープ関数へ
			Sleep(20);
		}
	}

	return msg.wParam;
}


///////ウインドウプロシージャ関数//////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 画像を読み込む準備
	HDC hdc;		// デバイスコンテキストハンドル
	PAINTSTRUCT ps; // ペイント構造体

	// バックバッファ用コンテキストハンドルと、ビットマップハンドルの準備
	static HDC hdc_back;	// 裏画面（バックバッファ）用デバイスコンテキストハンドル
	static HBITMAP hb_back;	// 裏画面（バックバッファ）用ビットマップハンドル

	switch (msg)
	{


	case WM_USER + 1:	// ゲームループ（SendMessageから呼ばれる）

		// 結果の再描画　WM_PAINTを実行する
		InvalidateRect(hWnd, NULL, false);
		return 0;

	case WM_PAINT:	// 画面を描画する時の処理

		hdc = BeginPaint(hWnd, &ps);	// 描画の開始
		Paint(hdc_back);				// Paint関数へ

		//バックバッファに保存された画像を表画面に描画
		BitBlt(hdc, 0, 0, 1100, 500, hdc_back, 0, 0, SRCCOPY);




		EndPaint(hWnd, &ps);			// 描画の終了
		return 0;

	case WM_CREATE:


		Init_Game();

		// 描画の開始
		hdc = GetDC(hWnd);
		// 互換デバイスコンテキストの作成
		hdc_back = CreateCompatibleDC(hdc);
		// 互換ビットマップの作成
		hb_back = CreateCompatibleBitmap(hdc, 1100, 500);
		// オブジェクトの選択
		SelectObject(hdc_back, hb_back);
		// 描画の終了
		ReleaseDC(hWnd, hdc);

		// ビットマップハンドルに画像データを読み込み保存しておく
		//シーン
		title_hb = (HBITMAP)LoadImage(NULL, TEXT("title.bmp"), IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		end_hb = (HBITMAP)LoadImage(NULL, TEXT("end.bmp"), IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		player_hb = (HBITMAP)LoadImage(NULL, TEXT("player.bmp"), IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//キャラクター
		hb_Player = (HBITMAP)LoadImage(NULL, TEXT("Player.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk_Right = (HBITMAP)LoadImage(NULL, TEXT("run1_right.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk2_Right = (HBITMAP)LoadImage(NULL, TEXT("run2_right.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk3_Right = (HBITMAP)LoadImage(NULL, TEXT("run3_right.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk4_Right = (HBITMAP)LoadImage(NULL, TEXT("run4_right.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk_Left = (HBITMAP)LoadImage(NULL, TEXT("run1_left.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk2_Left = (HBITMAP)LoadImage(NULL, TEXT("run2_left.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk3_Left = (HBITMAP)LoadImage(NULL, TEXT("run3_left.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Walk4_Left = (HBITMAP)LoadImage(NULL, TEXT("run4_left.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Jump = (HBITMAP)LoadImage(NULL, TEXT("Jump.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		hb_Tackl = (HBITMAP)LoadImage(NULL, TEXT("Tackl.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	


		return 0;

	case WM_DESTROY:	// 最後にウインドウが閉じられた時の処理
		// デバイスコンテキストの解放
		DeleteDC(hdc_back);
		// オブジェクトの解放 
		DeleteObject(hb_back);
		DeleteObject(title_hb);
		DeleteObject(end_hb);
		PostQuitMessage(0);
		return 0;

	default:	// デフォルト処理
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int Init_Game(){


	SceneNum = Title;

	player.x = 100;
	player.y = 200;
	player.width = 30;
	player.height = 60;
	player.attack_x = 30;
	player.attack_mukou = 90;
	
	//以下状態
	player.c_flg = STAND;
	//キャラクターアニメーション
	a_flg = A_STAND;
	a_cnt = 0;
	//タックルのフラグ
	x = 0;
	grv = 1.1;


	return 0;
}







/*
オブジェクト優先度

Player Move
　|
  V
Scroll			背景描画のため。
  |
  V
Item
  |
  V
Player　Oil
　|
　V
 Enemy
  |
  V
BLOCK
  |
  V
Player　Paint


*/

int Paint(HDC hdc)
{
	static int cc;
	if (SceneNum == Title){
		Paint_BG(hdc);		// 背景描画関数へ
		if (cc > 50){
			cc = Get_Key();
			if (cc == 1){
				SceneChanger();
				eobj = new(EnemyManager);
				iobj = new(ItemManager);
				scrobj = new(Scroll);
				blobj = new(Block);
				oilobj = new(OIL);
				pl_move_obj = new(MOVE);
				scrobj->Backimg_x = -(9  * CHIP_SIZE - 550);
				scrobj->Backimg_y = -(30 * CHIP_SIZE - 350);
				player.x = 240;
				player.y = 200;
			}
		}

	}
	else if (SceneNum == Stage1){
		
	pl_move_obj->C_sts(oilobj->c_sts, &oilobj->Oil_Gage);
	pl_move_obj->Move();

	//Scroll
	scrobj->toPoint(&player);
	scrobj->scroll_kansu(hdc);

	//Block
	blobj->toPoint(&player);
	blobj->block_scroll(scrobj->Backimg_x, scrobj->Backimg_y);
	int aa = blobj->block_kansu(hdc);
	if (aa == End){
		SceneNum = End;
		return 0;
	}

	//Enemy
	//-----------------------------
	eobj->chara_strc(&player);
	eobj->enemy_scroll(scrobj->BackMoveX, scrobj->BackMoveY);
	eobj->stage_coord(blobj->get_block_X(), blobj->get_block_Y());
	eobj->MainLoop(hdc);

	oilobj->Item(eobj->GetDeadflag());		//アイテムを取得してゲージに変化があるか判断する(敵の場合は−１される）

	eobj->GetDeadflag(0);

	//オイルゲージ量など更新処理
	oilobj->Player_Sts();
	oilobj->Oil_Sts();


	//Item
	iobj->chara_strc(&player);
	iobj->item_scroll(scrobj->BackMoveX, scrobj->BackMoveY);
	iobj->stage_coord(blobj->get_block_X(), blobj->get_block_Y());
	iobj->MainLoop(hdc);

	oilobj->Item(iobj->GetItemtype());		//アイテムを取得してゲージに変化があるか判断する
	DebugStringVal("%d", iobj->GetItemtype(), hdc, 300, 30, 20);

	iobj->GetItemtype(0);	//値の初期化。　しないとバグが出ます＾ｑ＾
	
	//オイルゲージ量など更新処理
	oilobj->Player_Sts();
	oilobj->Oil_Sts();


	//オイルとそのゲージの描画
	oilobj->Paint_Oil(hdc);
	oilobj->Paint_Gage(hdc);


	//移動量の最大値の抑制
	if (player.vx < -10)player.vx = -10;
	if (player.vy > 10)	player.vy = 10;

	player.x += player.vx;
	player.y += player.vy;

	//キャラが画面座標０より外へ出ないように抑制
	if (player.x < 0)	player.x = 0;
	if (player.y < 0)	player.y = 0;

	//以下描画処理

	//状態普通
	pl_paint_obj.char_strc(&player);
	if (oilobj->c_sts == HUTHU && a_flg == A_STAND){
		pl_paint_obj.Paint_Player_H(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK1_Right){
		pl_paint_obj.Paint_Walk1_H_Right(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK2_Right){
		pl_paint_obj.Paint_Walk2_H_Right(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK3_Right){
		pl_paint_obj.Paint_Walk3_H_Right(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK4_Right){
		pl_paint_obj.Paint_Walk4_H_Right(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_JUMP){
		pl_paint_obj.Paint_Jump_H(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_TACKL){
		pl_paint_obj.Paint_Tackl_H(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK1_Left){
		pl_paint_obj.Paint_Walk1_H_Left(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK2_Left){
		pl_paint_obj.Paint_Walk2_H_Left(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK3_Left){
		pl_paint_obj.Paint_Walk3_H_Left(hdc);
		player.width = 30;
	}
	if (oilobj->c_sts == HUTHU && a_flg == A_WALK4_Left){
		pl_paint_obj.Paint_Walk4_H_Left(hdc);
		player.width = 30;
	}

	//一番重い状態
	if (oilobj->c_sts == DEBU && a_flg == A_STAND){
		pl_paint_obj.Paint_Player_D(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK1_Right){
		pl_paint_obj.Paint_Walk1_D_Right(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK2_Right){
		pl_paint_obj.Paint_Walk2_D_Right(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK3_Right){
		pl_paint_obj.Paint_Walk3_D_Right(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK4_Right){
		pl_paint_obj.Paint_Walk4_D_Right(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_JUMP){
		pl_paint_obj.Paint_Jump_D(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_TACKL){
		pl_paint_obj.Paint_Tackl_D(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK1_Left){
		pl_paint_obj.Paint_Walk1_D_Left(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK2_Left){
		pl_paint_obj.Paint_Walk2_D_Left(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK3_Left){
		pl_paint_obj.Paint_Walk3_D_Left(hdc);
		player.width = 45;
	}
	if (oilobj->c_sts == DEBU && a_flg == A_WALK4_Left){
		pl_paint_obj.Paint_Walk4_D_Left(hdc);
		player.width = 45;
	}

	//一番中の上状態
	if (oilobj->c_sts == POTYA && a_flg == A_STAND){
		pl_paint_obj.Paint_Player_P(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK1_Right){
		pl_paint_obj.Paint_Walk1_P_Right(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK2_Right){
		pl_paint_obj.Paint_Walk2_P_Right(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK3_Right){
		pl_paint_obj.Paint_Walk3_P_Right(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK4_Right){
		pl_paint_obj.Paint_Walk4_P_Right(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_JUMP){
		pl_paint_obj.Paint_Jump_P(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_TACKL){
		pl_paint_obj.Paint_Tackl_P(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK1_Left){
		pl_paint_obj.Paint_Walk1_P_Left(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK2_Left){
		pl_paint_obj.Paint_Walk2_P_Left(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK3_Left){
		pl_paint_obj.Paint_Walk3_P_Left(hdc);
		player.width = 38;
	}
	if (oilobj->c_sts == POTYA && a_flg == A_WALK4_Left){
		pl_paint_obj.Paint_Walk4_P_Left(hdc);
		player.width = 38;
	}

	//中の下状態
	if (oilobj->c_sts == TYOIYASE && a_flg == A_STAND){
		pl_paint_obj.Paint_Player_Y(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK1_Right){
		pl_paint_obj.Paint_Walk1_Y_Right(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK2_Right){
		pl_paint_obj.Paint_Walk2_Y_Right(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK3_Right){
		pl_paint_obj.Paint_Walk3_Y_Right(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK4_Right){
		pl_paint_obj.Paint_Walk4_Y_Right(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_JUMP){
		pl_paint_obj.Paint_Jump_Y(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_TACKL){
		pl_paint_obj.Paint_Tackl_Y(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK1_Left){
		pl_paint_obj.Paint_Walk1_Y_Left(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK2_Left){
		pl_paint_obj.Paint_Walk2_Y_Left(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK3_Left){
		pl_paint_obj.Paint_Walk3_Y_Left(hdc);
		player.width = 22;
	}
	if (oilobj->c_sts == TYOIYASE && a_flg == A_WALK4_Left){
		pl_paint_obj.Paint_Walk4_Y_Left(hdc);
		player.width = 22;
	}

	//一番軽い状態
	if (oilobj->c_sts == GARI && a_flg == A_STAND){
		pl_paint_obj.Paint_Player_G(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK1_Right){
		pl_paint_obj.Paint_Walk1_G_Right(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK2_Right){
		pl_paint_obj.Paint_Walk2_G_Right(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK3_Right){
		pl_paint_obj.Paint_Walk3_G_Right(hdc);
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK4_Right){
		pl_paint_obj.Paint_Walk4_G_Right(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK1_Left){
		pl_paint_obj.Paint_Walk1_G_Left(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK2_Left){
		pl_paint_obj.Paint_Walk2_G_Left(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK3_Left){
		pl_paint_obj.Paint_Walk3_G_Left(hdc);
	}
	if (oilobj->c_sts == GARI && a_flg == A_WALK4_Left){
		pl_paint_obj.Paint_Walk4_G_Left(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_JUMP){
		pl_paint_obj.Paint_Jump_G(hdc);
		player.width = 15;
	}
	if (oilobj->c_sts == GARI && a_flg == A_TACKL){
		pl_paint_obj.Paint_Tackl_G(hdc);
		player.width = 15;
	}
	}
	else if (SceneNum == End){
		Paint_BG(hdc);
		if (cc > 50){
			cc = Get_Key();
			if (cc == 1){
				SceneChanger();
				delete eobj;
				delete iobj;
				delete scrobj;
				delete blobj;
				delete pl_move_obj;
				delete oilobj;
			}
		}
		
	}
	cc++;

	return 0;
}



///// キー入力関数 ///////////////////////////////////////////////////////
int Get_Key()
{
	GetKeyboardState(key_buff);
	key_input_buff = 0;

	if (key_buff[VK_SPACE] & 0x80){
		key_input_buff |= KEY_SPACE;
		return 1;
	}

	return 50;
}

int Paint_BG(HDC hdc){
	HDC hdc_work;
	hdc_work = CreateCompatibleDC(hdc);
	if (SceneNum == Title){
		SelectObject(hdc_work, title_hb);		// オブジェクトの選択
		BitBlt(hdc, 0, 0, 1100, 350, hdc_work, 0, 0, SRCCOPY);
	}
	else if (SceneNum == End){
		SelectObject(hdc_work, end_hb);		// オブジェクトの選択
		BitBlt(hdc, 0, 0, 1100, 350, hdc_work, 0, 0, SRCCOPY);
	}


	DeleteDC(hdc_work);// デバイスコンテキストの解放
	return 0;
}

int SceneChanger(){
	switch (key_input_buff)
	{
	case 32:
		if (SceneNum == Title){
			SceneNum = Stage1;
		}
		if (SceneNum == End){
			SceneNum = Title;
		}
		break;
	default:
		break;
	}
	return 0;
}

