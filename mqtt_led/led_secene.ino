void black()
{
  for(byte x =0; x< NUMPIXELS;x++)
  {
     pixels.setPixelColor(x, pixels.Color(0, 0, 0));
  }
  pixels.show();
}

byte timer_fade_in = 2;
void fade_in()
{
  static int pixel_print;
  static byte counter = 0;
  counter++;
  Serial.println(pixel_print);
  if(counter == timer_fade_in)
  {
      counter =0;
      pixel_print++;
      if(NUMPIXELS/2 < pixel_print)
      {
        pixel_print =0; 
      }
  }
  for(byte x =0; x< NUMPIXELS/2+1;x++)
  {
    if(x <= pixel_print)
    {
       pixels.setPixelColor(x, pixels.Color(0, 150, 0));
       pixels.setPixelColor(NUMPIXELS-x, pixels.Color(0, 150, 0));
    }
    else
    {
       pixels.setPixelColor(x, pixels.Color(0, 0, 0));
       pixels.setPixelColor(NUMPIXELS-x, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();
}

byte timer_hate_in = 5;
void hate()
{
  static bool pixel_print;
  static byte color = 0;
  static byte counter = 0;
  
  counter++;
  if(counter == timer_hate_in)
  {
      counter = 0;
      color ++;
      if(color == 250)
      {
        color = 0;
      }
      pixel_print =! pixel_print;
      if(pixel_print == 0)
      {
        for(byte x =0; x< NUMPIXELS;x++)
        {
          pixels.setPixelColor(x, pixels.Color(color, 0, 0));
        }
      }
      else
      {
        for(byte x =0; x< NUMPIXELS;x++)
        {
          pixels.setPixelColor(x, pixels.Color(0, 0, 0));
        }
      }
        
  }
  
  pixels.show();
}


byte color_confeti[2][3][3]=
{ //RED GREEN BLUE
  {
  {150,0,15}, //BLUE
  {150,0,0},  //red
  {150,15,0},  //green
  },
   {
  {120,30,0}, //BLUE
  {125,25,0},  //red
  {110,35,0},  //green
  },
};



byte speed_conf = 15;
byte conf_fading_min = 30;
byte conf_fading_max = 60;
byte rand_confetin = 5;
void confeti(byte color )
{
  static byte array_confeti_pos[NUMPIXELS];
  static byte array_confeti_color[NUMPIXELS];
  static bool bar_pos = 0;
  static byte ss = 0;
  ss++;
  if(ss == speed_conf)
  {
    ss=0;
    for(byte y= 0;y<NUMPIXELS; y++)
    {
      if(random(0,rand_confetin) == 1)
      {
        array_confeti_pos[y] = random(conf_fading_min,conf_fading_max);
        array_confeti_color[y] = random(0,3);
      }
    }
    
  }

  for(int i=0;i<NUMPIXELS;i++)
  {
    if(array_confeti_pos[i] > 0)
    {
        array_confeti_pos[i]--;
        int a,b,c = 0;
        a = (color_confeti[color][array_confeti_color[i]][0] * array_confeti_pos[i] )/conf_fading_max;
        if(a < 0 )
        {
          a = 0;
        }
        b = (color_confeti[color][array_confeti_color[i]][1] * array_confeti_pos[i] )/conf_fading_max;
        if(b < 0 )
        {
          b = 0;
        }
        c = (color_confeti[color][array_confeti_color[i]][2] * array_confeti_pos[i] )/conf_fading_max;
        if(c < 0 )
        {
          c = 0;       
        }      
        pixels.setPixelColor(i, pixels.Color(a,b,c));   
    }
    else
    {
       pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }  
  pixels.show();
}
