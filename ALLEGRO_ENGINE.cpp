#include <allegro.h>

// DECLARACOES INICIAIS //
int sair=0; void sair_allegro() {sair=1;}; 
int timer=0; void tempo() {timer++;};
int Ctrl_FPS=60; float delay=0;

// INICIALIZACAO ALLEGRO //
int main()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse(); 
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 480, 0, 0);
  install_int_ex(tempo, BPS_TO_TIMER(60)); //60fps
  set_window_title("ALLEGRO MINIMAL");
  set_close_button_callback( sair_allegro );
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  BITMAP *buffer = create_bitmap(1280,480);
  BITMAP *FakeGameScreen = create_bitmap(320,240);
  BITMAP *bg1_spr = load_bitmap("bg1_spr.pcx", NULL);
  BITMAP *bg2_spr = load_bitmap("bg2_spr.pcx", NULL);
  BITMAP *bg3_spr = load_bitmap("bg3_spr.pcx", NULL);
  FONT *font_20 = load_font("font_20.pcx", NULL, NULL);
  float camera_x=0;
  float camera_y=0;
  float camera_w=320;
  float camera_h=240;
  float bg1_x=0;
  float bg1_y=0;
  float bg1_w=bg1_spr->w;
  float bg1_h=bg1_spr->h;
	float px=camera_w/2;
  float py=camera_h-50;
  float bg2_x=0;
  float bg2_y=0;
  float bg2_w=bg2_spr->w;
  float bg2_h=bg2_spr->h;
	float bg3_x=0;
  float bg3_y=0;
  float bg3_w=bg3_spr->w;
  float bg3_h=bg3_spr->h;
  float parallax_factor;
  
  // LOOP DE JOGO // 
  while (sair==0)
  {
  delay=timer; 
	if (key[KEY_ESC]) {sair=1;}; 
	clear_to_color(buffer,makecol(100,149,237)); //fundo azul
	
	/* < SEU CODIGO AQUI! > */
	//inputs
	int velocidade=3;
	if (key[KEY_RIGHT]) { px+=velocidade; } 
	if (key[KEY_LEFT] ) { px-=velocidade; } 
	if (key[KEY_UP]   ) { py-=velocidade; } 
	if (key[KEY_DOWN] ) { py+=velocidade; } 
	//limites dos inputs
	int limite=15;
	if (px<limite) { px=limite; }
	if (px>bg1_w-limite) { px=bg1_w-limite; }
	if (py<limite) { py=limite; }
	if (py>bg1_h-limite) { py=bg1_h-limite; }
	//camera
	camera_x=px-160;
	if(camera_x<0) { camera_x=0; }
	if(camera_x>bg1_w-320) { camera_x=bg1_w-320; }
	camera_y=py-240;
	if(camera_y<0) { camera_y=0; }
	if(camera_y>bg1_h-240) { camera_y=bg1_h-240; }
	camera_w=camera_x+320;
	camera_h=camera_y+240;
	//BG1
	draw_sprite(buffer, bg1_spr, bg1_x, bg1_y);
	rect(buffer, bg1_x, bg1_y, bg1_w, bg1_h, makecol(000,000,000));
	parallax_factor = ( ( ( camera_x*100 ) / bg1_w ) / 100 ) *2 *-1;
	//BG2
	bg2_x = (bg2_w-640) * parallax_factor;
	rect(buffer, bg2_x, bg2_y, bg2_x+bg2_w, bg2_y+bg2_h, makecol(000,000,000));
	draw_sprite(buffer, bg2_spr, bg2_x, bg2_y);
	//BG3
	bg3_x = (bg3_w-640) * parallax_factor;
	rect(buffer, bg3_x, bg3_y, bg3_x+bg3_w, bg3_y+bg3_h, makecol(000,000,000));
	draw_sprite(buffer, bg3_spr, bg3_x, bg3_y);
	//draw
	rect(buffer, camera_x, camera_y, camera_w, camera_h, makecol(255,255,255));
	circlefill(buffer, px, py, 10, makecol(255,255,255));
	//FakeGameScreen
	rectfill(buffer, 955-5, 235-5, 1280, 480, makecol(255,255,255));
	blit(buffer, FakeGameScreen, camera_x, camera_y, 0, 0, camera_w, camera_h);
	blit(FakeGameScreen, buffer, 0, 0, 955, 235, FakeGameScreen->w, FakeGameScreen->h);
  //debug
	textprintf_ex(buffer, font_20, 5,  20+240, makecol(255,255,000), -1, "Sistema de Camera e Parallax Scrolling");
	textprintf_ex(buffer, font_20, 5,  50+240, makecol(255,255,255), -1, "Px: %.0f, Py: %.0f, Fator Parallax: %.2f", px, py, parallax_factor);
	textprintf_ex(buffer, font_20, 5,  80+240, makecol(255,255,255), -1, "CAMERAx: %.2f CAMERAy: %.2f", camera_x, camera_y);
	textprintf_ex(buffer, font_20, 5, 110+240, makecol(255,255,255), -1, "BG1x: %.2f BG1y: %.2f BG1w: %.2f BG1h: %.2f", bg1_x, bg1_y, bg1_w, bg1_h);
	textprintf_ex(buffer, font_20, 5, 140+240, makecol(255,255,255), -1, "BG2x: %.2f BG2y: %.2f BG2w: %.2f BG2h: %.2f", bg2_x, bg2_y, bg2_w, bg2_h);
	textprintf_ex(buffer, font_20, 5, 170+240, makecol(255,255,255), -1, "BG3x: %.2f BG3y: %.2f BG3w: %.2f BG3h: %.2f", bg3_x, bg3_y, bg3_w, bg3_h);
	textprintf_ex(buffer, font_20, 5, 200+240, makecol(255,255,255), -1, "Distancia Percorrida: %.0f%%", (parallax_factor*100)*-1);
	textprintf_centre_ex(buffer, font, 1115, 240, makecol(255,255,255), -1, "tela do jogo");
	//double buffer
	draw_sprite(screen, buffer, 0, 0);
		
  // FINALIZACOES //
	while(timer==delay){};	
	clear(buffer);
  }; 
  destroy_bitmap(buffer);
  return 0;
};
END_OF_MAIN();
