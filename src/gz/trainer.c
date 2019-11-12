#include <stdlib.h>
#include <math.h>
#include "gfx.h"
#include "gz.h"
#include "menu.h"
#include "resource.h"
#include "settings.h"
#include "z64.h"
#include "trainer.h"


struct roll roll;
struct sidehop sidehop;
struct hess hess;

bomb_t bomb1;

_Bool is_rolling()
{
  if((z64_link.current_animation == ANIM_ROLL) || (z64_link.current_animation == ANIM_ROLL_SHIELD))
    return 1;
  else
    return 0;
}

_Bool is_sidehopping()
{
  if((z64_link.current_animation == ANIM_SIDEHOP_L) || (z64_link.current_animation == ANIM_SIDEHOP_R))
    return 1;
  else
    return 0;
}

_Bool is_landing()
{
  if((z64_link.current_animation == ANIM_LANDING_L) || (z64_link.current_animation == ANIM_LANDING_R))
    return 1;
  else
    return 0;
}

_Bool roll_pressed()
{
  if(is_rolling() && (z64_link.animation_timer == 0))
    return 1;
  else
    return 0;
}

_Bool sidehop_pressed()
{
  if(is_sidehopping() && (z64_link.animation_timer == 0))
    return 1;
  else
    return 0;
}

_Bool a_pressed()
{
  uint16_t pad = z64_game.common.input[0].raw.pad;
  if(pad & 0x8000){
    return 1;
  }else{
    return 0;
  }
}

void update_roll()
{
  if(roll_pressed() && !roll.timer_active){
    roll.timer_active = 1;
    roll.is_first_roll = 1;
    roll.last_roll_frame = 0;
  } else if (roll_pressed() && roll.timer_active){
    roll.is_first_roll = 0;
    roll.last_roll_frame = roll.timer;
    roll.timer = 0;
  }

  if(roll.timer_active){
    roll.timer++;
  } else{
    roll.timer = 0;
  }

  if(roll.timer == 30){
    roll.timer_active = 0;
  }
}

void check_streak()
{
  if((roll_pressed()) && (roll.last_roll_frame == 16)){
    roll.streak++;
  }
  if(roll.last_roll_frame != 16){
    roll.streak = 0;
  }
}

void update_sidehop()
{
  if(sidehop_pressed()){
    sidehop.sidehop_timer = 0;
    sidehop.sidehop_timer_active = 1;
  }

  if(sidehop.sidehop_timer_active && (a_pressed())){
    sidehop.a_press = sidehop.sidehop_timer;
  }

  if(sidehop.sidehop_timer_active){
    sidehop.sidehop_timer++;
  }

  if(is_landing()){
    sidehop.land_timer_active = 1;
  }

  if(sidehop_pressed() && sidehop.land_timer_active){
    sidehop.result = sidehop.land_timer;
    sidehop.land_timer = 0;
    sidehop.land_timer_active = 0;
  }

  if(sidehop.land_timer_active){
    sidehop.land_timer++;
  }

  if(sidehop.land_timer == 20){
    sidehop.land_timer = 0;
    sidehop.sidehop_timer = 0;
    sidehop.land_timer_active = 0;
    sidehop.sidehop_timer_active = 0;
  }

}

static void update_bomb_timers(bomb_t* bomb)
{
  if(bomb->instance->timer > 0){
    bomb->timer++;
  } else {
    bomb->timer = 0;
  }
}

void update_bombs()
{
  bomb1.instance = (z64_bomb_t*)z64_game.actor_list[3].first;
  if(bomb1.instance){
    update_bomb_timers(&bomb1);
    bomb1.distance_from_link = bomb1.instance->common.dist_from_link_xz;
  }else{
    bomb1.timer = 0;
  }
}

void hess_roll(bomb_t* bomb)
{
  if(bomb->instance){
    if((bomb->timer > 55) && roll_pressed()){
      hess.roll_frame = bomb->timer;
    }
    if(bomb->timer == 1){
      hess.roll_frame = 0;
    }
  }
}

void hess_setup(bomb_t* bomb)
{
  if(bomb->instance){
    if((bomb->instance->common.attached_a == 0) && (z64_link.linear_vel == 0) && (bomb->timer > 60)){
      int dist_rounded;
      dist_rounded = round(bomb->distance_from_link);
      if (dist_rounded == 63){
        hess.setup = HESS_ROLL_INSTA;
      }
    }
    if(bomb->timer == 1){
      hess.setup = HESS_UNKOWN;
    }
  }
}

/* set color functions */
void set_rgb_green()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0x00, 0xFF, 0x2F, 0xFF));
}

void set_rgb_lgreen()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0x7D, 0xFF, 0x95, 0xFF));
}

void set_rgb_yellow()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xEF, 0xFF, 0x20, 0xFF));
}

void set_rgb_orange()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xFF, 0xA5, 0x10, 0xFF));
}

void set_rgb_red()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xFF, 0x24, 0x24, 0xFF));
}

void set_rgb_white()
{
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xFF, 0xFF, 0xFF, 0xFF));
}
