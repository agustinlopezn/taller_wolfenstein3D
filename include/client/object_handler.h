//
// Created by andy on 4/12/20.
//

#ifndef TP_WOLFENSTEIN_OBJECTDRAWER_H
#define TP_WOLFENSTEIN_OBJECTDRAWER_H

#include "sdl_window.h"
#include <vector>
#include <map>
#include <limits>
#include "image_parser.h"
#include "image_info.h"
#include "calculator.h"
#include "client_map.h"

class ObjectHandler {
public:
    ObjectHandler(SdlWindow& _window, ObjectInfoProvider& object_info_provider,
                  std::map<double, double>& wall_distance_info,
                  std::map<int, std::pair<int, int>>& _floor_info,
                  std::vector<double>& angles_list, ClientMap& map);
    void findObjectProportions(MapInfo& drawing_info, double distance,
                               double pl_ob_angle, Area& screen_area);

    int findXPosForObject(double pl_ob_beta, int object_width);
    int findYPosForObject(int ray_no, double pl_ob_angle, double distance,
                          int object_height);
    int findObjectHeight(double distance, MapInfo& drawing_info);
    int findObjectWidth(double distance, MapInfo& drawing_info);
    int findRayNumberForAngle(double beta);

    Area assembleScreenArea(MapInfo& drawing_info, double distance,
                            double pl_ob_angle);
    void loadObjects(int x, int y, double player_angle);
    void setDimensions(int width, int height);
private:
    SDL_Renderer* renderer;
    SdlWindow& window;
    ObjectInfoProvider& object_info_provider;
    int window_width;
    int window_height;
    int width_prop;
    int height_prop;
    int map_grid_size;
    std::map<double, double>& wall_distance_info;
    std::map<int, std::pair<int, int>>& floor_info;
    std::vector<double>& angles_list;
    ClientMap& map;
    int safe_distance = 30;
    bool shouldDraw(double player_angle, double os_angle, double of_angle,
                    double& diff_angle);
    bool shouldDraw_borderCase(double os_angle, double of_angle,
                               double fov_starting_angle,
                               double fov_finishing_angle, double& diff_angle);
    double getObjectAngle(int p_x, int p_y, std::pair<int, int> o_pos);
    std::pair<int, int> projectObjectOnMap(Drawable& object,
                                           double player_angle);
    double calculateObjectStartingXPos(double os_angle, double of_angle,
                                       double diff_angle);
    void renderObject(int x_pos, int y_pos, double player_angle,
                      double object_angle, double x_prop, Drawable& object);
    double findWallDistanceForAngle(double angle);
    double getGammaAngle(double player_angle, double object_angle);
    bool blockedByWall(double angle, double distance);
};


#endif //TP_WOLFENSTEIN_OBJECTDRAWER_H