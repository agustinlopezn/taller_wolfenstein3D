//
// Created by andy on 3/1/21.
//

#ifndef TP_WOLFENSTEIN_UI_DRAWER_H
#define TP_WOLFENSTEIN_UI_DRAWER_H

#include <string>
#include "area.h"
#include "client_player.h"
#include "object_info_provider.h"
#include "sdl_window.h"

class UIDrawer {
private:
    SdlWindow& window;
    int starting_point;
    int height;
    int width;
    int box_starting_point = 0;
    int text_starting_point = 0;
    int h_padding = 10;
    int v_padding = 0;
    SdlSprite player_face;
    ObjectInfoProvider& info_provider;
    SDL_Texture* createMessage(const std::string& message, Area& msg_area,
                               Area& screen_area, bool fill_text_area);
    void drawPlayersLevel(int players_level);
    void drawPlayersScore(int players_score);
    void drawPlayersLives(int players_lives);
    void drawPlayersHealth(int players_health);
    void drawPlayersAmmo(int players_ammo);
    void drawPlayersImage();
    void drawPlayersWeaponIcon(int player_weapon);
    void drawPlayersEquippedWeapon(int weapon_number);
    void drawBox(const std::string& message, int value);
    void fillArea(Area area, int r, int g, int b, int a);
    void fillAreaWithBorder(Area area, int r, int g, int b, int a);
    void fillTextArea(TTF_Font* font, const std::string& message,
                      Area& screen_area);
    void putTextureAt(SDL_Texture* texture, Area src, Area dest);
    Area assembleScreenWeaponArea(ObjectInfo& object_info);
    SDL_Texture* getWeaponSprite(ObjectInfo& o_i, Area& image_area);
public:
    UIDrawer(ObjectInfoProvider& _info_provider, SdlWindow& _window);
    void setDimensions(int _starting_point, int _height, int _width);
    void drawPlayerUI(ClientPlayer& player);
    void drawPlayersWeapon(int weapon_number);

};


#endif //TP_WOLFENSTEIN_UI_DRAWER_H
