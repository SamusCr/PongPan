/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0

*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

typedef enum  Estates { LOGO_SCREEN = 0, TITLE_SCREEN, GAMPLEAY_SCREEN, EXIT_SCREEN} Estates;

//Tamaño de ventana
const static int screenWidth = 800;
const static int screenHeight = 450;

//Metodo para hacer update
static void UpdateDrawing(void);

//bola
static float ballRadi = 20.0f;
static Vector2 ballPosition = {400,225};
static int velocidadBolaX = 6;
static int velocidadBolaY= -4;


static float alphaTitulo = 0.0f;



/*  //El "Super Cool Pon Pang Ultra Turbo" del juego    
char titulo[]  = "Super Cool Pon Pang Ultra Turbo"; */
//El sub"Super Cool Pon Pang Ultra Turbo" del juego
/*  char subtitle[] = "& Ruckles 2"; */

/* //Aviso de pres start   
char pressTitle = "Press Enter to play"; */
/* //Texto de las reglas
char  textReglas[]= "Intenta hacer el maximo de puntuacion antes que el rival llegue a 100 puntos"; */

/*  //Texto Endgame 
char  textFinal[]= "Felicidades has conseguido "; */
/* //Texto para volver a jugar
char textReplay[] =  "PARA SEGUIR JUGANDO PULAS ENTER";
//Texto para salir
char textExit[] = "PARA SALIR PULSA ESC"; */
/* 
char textJugando[] = "Para abandonar partida pulsa Enter - Para cerrar juego pulsa ESC";

char textPausa[] = "GAME PAUSED"; */



//Array de static Colores para cambiar el "Super Cool Pon Pang Ultra Turbo"
static Color Colors[6] ={ GREEN, BLUE, MAGENTA, RED , VIOLET, PINK};
static int actualColor = 0;

//Jugador
static Rectangle player = { 0, 225 - 50 , 25 ,100};

//enemigo
static Rectangle enemigo = { 800 -25 , 225 - 50 , 25 ,100};
static int velocidadenemigo = 4;
static int distanciaRacEne =  200 + 400;

//Contador de frames desde que se inició el juego
static int frameCount = 0;


//Puntuaciones
static int scoreplayer = 0;
static int scoreEnemie = 0;


static bool startGame = false;
//En que parte del juego estamos
Estates Game_Sate = LOGO_SCREEN;
static bool pusedGame = false;

//Inicializar las cargas
Texture2D logo ={0};
Font fuenteTexto ={0};
Music MusicaFondo = {0};
Sound startSound = {0};
Sound pongSound = {0};
//------------------------------------------------------------------------------------
// Program main entry postatic int
//------------------------------------------------------------------------------------

int main(void)
{
        // Initialization
        //--------------------------------------------------------------------------------------       
        //Inicializar la pantalla
        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
       
        //Inicializar el Audio Device 
        InitAudioDevice();    
        
        //Cargar textura del logo
        logo = LoadTexture("Resource/logo_raylib.png");    
        
        //Cargar Fuente de texto
        fuenteTexto = LoadFont("Resource/texto_fuente.png");
        //Cargar Musica de fondo
        MusicaFondo = LoadMusicStream("Resource/qt-plimp.xm");
        PlayMusicStream(MusicaFondo);
        //Efectos de sonido
        startSound = LoadSound("Resource/start_sound.wav");
        pongSound = LoadSound("Resource/pong_sound.wav");
        
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
 
        //--------------------------------------------------------------------------------------
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
        // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // UpdateDrawing
          UpdateDrawing();      
    }
 #endif   
     // De-Initialization
    //Desinicializar Sonido de pong_sound    
    UnloadSound(pongSound);
    //Desinicializar Sonido start
    UnloadSound(startSound);
    //Desinicializar muisica
    UnloadMusicStream(MusicaFondo);
    //Desinicializar la textura del logo
    UnloadTexture(logo);
    //Desinicializar fuente de texto
    UnloadFont(fuenteTexto);
    //Cerrar EL sistema de audio
    CloseAudioDevice();
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
     //--------------------------------------------------------------------------------------
    return 0;
}

static void UpdateDrawing(void)
{
      //----------------------------------------------------------------------------------
      //Contador de frames que han pasado durante la partida
        frameCount++;
       
        UpdateMusicStream(MusicaFondo);
       //Logica de las escena del juego 
        switch (Game_Sate)
        {
            //Escena inicial del logo
            case LOGO_SCREEN:
            {
                //Efecto de aparción del titulo aumentando su Alpha
                alphaTitulo += (1.0f/100.0f);
                
              //Si ya aparece el titulo empezar la escena del titulo
                if(alphaTitulo > 1.0f)
                {
                    alphaTitulo = 1.0f;
                    Game_Sate = TITLE_SCREEN;
                   
                }
                
            }break;
            //Escena del titulo
             case TITLE_SCREEN:
            {
                //Pulsar Enter para empezar juego
                if(IsKeyPressed(KEY_ENTER))
                {

                    PlaySound(startSound);
                    startGame = true;
                    
                }
                
                //Compruebo si ya se puede empezar el juego.
                //Lo hago de esta forma aumentar el contador de frames. De esta forma aumentamos el parpadeado de los "Super Cool Pon Pang Ultra Turbo"s i el "press enter" para .
                if(  startGame == true)
                {
                    frameCount +=10;
                    //Empezar la escena del juego cuando acabe el sonido de Start
                    if(!IsSoundPlaying(startSound))
                    {                    
                        Game_Sate = GAMPLEAY_SCREEN; 
                        startGame = false;
                    }
                }
                            
            }break;
            //Escena del juego 
             case GAMPLEAY_SCREEN:
            {
            //En caso de no estar pausado el juego empezar
            if(!pusedGame)
            {
                
                //Movimiento de la bola
                ballPosition.x = ballPosition.x + velocidadBolaX; 
                ballPosition.y = ballPosition.y + velocidadBolaY;
                
                
                //Rebote en las paredes de la bola
                if(((ballPosition.x + ballRadi) > screenWidth) || ((ballPosition.x - ballRadi) < 0)) 
                { 
                    PlaySound(pongSound);
                    velocidadBolaX *= -1; 
                    
                }
                else if(((ballPosition.y + ballRadi) > screenHeight) || ((ballPosition.y - ballRadi) < 0)) 
                { 
                    PlaySound(pongSound);
                    velocidadBolaY *=  -1; 
                }


                //Controles jugador
                if(IsKeyDown(KEY_UP))
                {
                    player.y -= 8;
                }
                if(IsKeyDown(KEY_DOWN))
                {
                    player.y +=  8;
                }
                
                //Clampear jugador
                if(player.y <= 0) 
                {
                    player.y =0;
                }
                else if (player.y + player.height> screenHeight) 
                {
                    player.y = screenHeight-player.height;
                }
                    
                    
                //IA enemigo
                if((enemigo.y < ballPosition.y) && ballPosition.x > (distanciaRacEne ))
                {
                    enemigo.y += velocidadenemigo;
                }
                else if ((enemigo.y >= ballPosition.y) )
                {
                    enemigo.y -= velocidadenemigo;
                }
                
                 if(enemigo.y < 0) 
                {
                    enemigo.y =0;
                }
                else if (enemigo.y > screenHeight- enemigo.height) 
                {
                    enemigo.y = screenHeight-enemigo.height;
                }
            
                 else if (enemigo.y > screenHeight- enemigo.height) 
                {
                    enemigo.y = screenHeight-enemigo.height;
                }
                    
                    
                //Detección de colisiones de la pelota i los jugadores. Me gustaría revisar en un futuro porque se buega cuando pongo el CheckCollisionCircleRec
                 if(CheckCollisionPointRec ((Vector2){ballPosition.x - ballRadi, ballPosition.y}, player))
                {
                    PlaySound(pongSound);
                    velocidadBolaX *=  -1; 
                }
                    
                else if(CheckCollisionPointRec ((Vector2){ballPosition.x + ballRadi, ballPosition.y}, enemigo))
                {
                    PlaySound(pongSound);
                    velocidadBolaX = velocidadBolaX * -1; 
                }
                
                  if(IsKeyPressed(KEY_ENTER))
                {
                    Game_Sate = EXIT_SCREEN;
                    
                }
             
                
                //Obtener puntos
                // Si golpea la del enemigo punta del enemigo, al llegar a 100 puntos se pierde
                
                if((ballPosition.x - ballRadi) < 0) 
                { 
                   
                    scoreEnemie += 10;
                   
                     if(scoreEnemie >= 100 )
                    {
                        
                        Game_Sate = EXIT_SCREEN;
                        
                    }
                }
                //Si la pelota golpea la pared del enemigo punto para el jugador i aumenta la dificultad aumentando la distancia de reacción. Hay un maximo, si se llega amuneta la velocidad.
                else if ((ballPosition.x + ballRadi) > GetScreenWidth()) 
                {
                   
                    scoreplayer += 10;
                    
                   if(distanciaRacEne > GetScreenWidth()/10) 
                   {
                       distanciaRacEne -= 50;
                   }
                   else
                   {
                       velocidadBolaX += 2;
                       velocidadenemigo += 2;
                   }
                }
                
            }
                   
           //Botón de pausa
            if (IsKeyPressed(KEY_P)) 
            {                
                pusedGame = !pusedGame;   
            }
            
            }break; 
                      
            //Escena final del juego
             case EXIT_SCREEN:
            {
                //Boton para volver a ir al titulo
                if(IsKeyPressed(KEY_ENTER))
                {                    
                    Game_Sate = TITLE_SCREEN;                     
                }
                
                            
            }  break;
            default: break;           
    }
       
        
        
        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        //limpiador de pantalla
        ClearBackground(RAYWHITE);
        //Logica de las escenas en el dibujado
        switch (Game_Sate)
        {
            //Escena del logo
            case LOGO_SCREEN:
            {               
                //Dibujado de logo con canvio de transparencia para dar efecto
                DrawTexture(logo, GetScreenWidth()/2 - logo.width/2 , GetScreenHeight()/2 - logo.height/2 , Fade(WHITE,alphaTitulo));                          
            }break;
            
            //Esceba del titulo principal
             case TITLE_SCREEN:
            {
                
                //Contar frames para saber cuando cambiar el static Color del "Super Cool Pon Pang Ultra Turbo" de texto
                if((frameCount)%5 == 0)
                {
                  //Controlar que no nos pasemos de el array de static Colores
                  if(actualColor < 5)
                    {
                        //Passar al siguiente static Color
                        actualColor++;
                        
                    }
                    else 
                    {
                        //Si nos hemos pasado ir al principio del array
                        actualColor =0;
                    }
                }
                //Dibujar el TItulo               
                DrawTextEx(fuenteTexto,"Super Cool Pon Pang Ultra Turbo",(Vector2){GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"Super Cool Pon Pang Ultra Turbo",40,1).x/2, GetScreenHeight()/10 },40,1, Colors[actualColor]);
                
                //Dibujar el Subtitulo
                DrawTextEx(fuenteTexto,"& Ruckles 2",(Vector2){GetScreenWidth()/2 + (MeasureTextEx(fuenteTexto,"Super Cool Pon Pang Ultra Turbo",40,1).x/2 - MeasureTextEx(fuenteTexto,"& Ruckles 2",20,1).x), GetScreenHeight()/10 +MeasureTextEx(fuenteTexto,"Super Cool Pon Pang Ultra Turbo",40,1).y },20,1,RED);
               
                //Dibujar las reglas
                DrawTextEx(fuenteTexto,"Intenta hacer el maximo de puntuacion antes que el rival llegue a 100 puntos",(Vector2){GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"Intenta hacer el maximo de puntuacin antes que el rival llegue a 100 puntos",18,1).x/2, GetScreenHeight()- 120 },18,1, GRAY);
                //QUe se deibuje cada 2 frames para dar el estilo de parpadeo
                if((frameCount/30)%2)
                {                    
                     DrawTextEx(fuenteTexto,"Press Enter to play",(Vector2){GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"Press Enter to play",20,1).x/2, GetScreenHeight()/ 2 },20,1, BLACK);
                }

                            
            }break;
            //Escena del juego
             case GAMPLEAY_SCREEN:
            {
                DrawCircleV(ballPosition, ballRadi, GREEN);
                
                //Dibujar jugador
                DrawRectangleRec (player, BLUE);
                
                //Dibujar enemigo
                 DrawRectangleRec (enemigo, RED);
                 
                //Dibujar las puntuaciones
                DrawText(TextFormat("%04i", scoreplayer), 100, 10, 30, BLUE);
                DrawText(TextFormat("%04i", scoreEnemie), GetScreenWidth() - 200, 10, 30, RED);
                //Dibujar aviso de que se puede cerrar o dar enter para pasar
                DrawText("Para abandonar partida pulsa Enter - Para cerrar juego pulsa ESC", GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"Para abandonar partida pulsa Enter - Para cerrar juego pulsa ESC",10,1).x/2, 10 , 10, GRAY);
                
                //Linea de activación del enemigo
                DrawLine( distanciaRacEne, 0, distanciaRacEne, screenHeight, GRAY);
                //Solo dibujar esto si el juego esta en pausa
                if(pusedGame)
                {
                    
                    DrawText("GAME PAUSED", GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"GAME PAUSED",30,1).x/2, GetScreenHeight()/2, 30, RED);
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(WHITE, 0.8f));
                   
                }
                
            }break; 
                      
             //Escena del final del juego
             case EXIT_SCREEN:
            {
                
                
                //Mensaje final del juego
                DrawTextEx(fuenteTexto, "Felicidades has conseguido ",(Vector2){GetScreenWidth()/2- MeasureTextEx(fuenteTexto, "Felicidades has conseguido ",30,1).x/2 , GetScreenHeight()/ 2- MeasureTextEx(fuenteTexto, "Felicidades has conseguido ",30,1).y/2},30,1,BLACK);
                 //Puntación final
                DrawTextEx( fuenteTexto, TextFormat("%04i", scoreplayer),(Vector2){GetScreenWidth()/2 - MeasureTextEx(fuenteTexto,TextFormat("%04i", scoreplayer),30,1).x/2, GetScreenHeight()/ 2- MeasureTextEx(fuenteTexto, "Felicidades has conseguido ",30,1).y/2 + MeasureTextEx(fuenteTexto, "Felicidades has conseguido ",30,1).y/2 + 10 }, 30,1, GREEN);
                 
                 //Parpadeo
                 if((frameCount/30)%2)
                {
                    //Explicación de que hacer para repetir o salid del juego
                    DrawTextEx(fuenteTexto, "PARA SEGUIR JUGANDO PULAS ENTER",(Vector2){(GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"PARA SEGUIR JUGANDO PULAS ENTER",20,1).x/2), GetScreenHeight()/10}, 20,1, BLACK);
                    DrawTextEx(fuenteTexto, "PARA SALIR PULSA ESC", (Vector2){(GetScreenWidth()/2- MeasureTextEx(fuenteTexto,"PARA SALIR PULSA ESC",20,1).x/2),(GetScreenHeight()/10 + MeasureTextEx(fuenteTexto,"PARA SALIR PULSA ESC",20,1).y +10)},20 ,1 , BLACK);
                }
                            
            }  break;  
            default: break;
            
        //----------------------------------------------------------------------------------
        }
        
        EndDrawing();
    }
       




