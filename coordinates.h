#ifndef COORDINATES_H
#define COORDINATES_H

#include <Arduino.h>
#include <EEPROM.h>

struct Stop {
    float lat;
    float lon;
    uint8_t audioNum;
    uint8_t radius;
};

struct RouteStop {
    uint16_t stopIndex;
    uint8_t afterAudio;
    uint16_t afterDelay;
};

struct Route {
    uint8_t dispNum;
    uint8_t stopsNum;
    RouteStop* steStops;
    RouteStop* etsStops;
};

const Stop stops[] PROGMEM = {
    {0, 0, 0, 0},                  // 0 - empty stop (no stop)
    {51.22395, 33.16785, 1, 35},   // 1 - 1-й пров. Залізничної
    {51.22464, 33.15988, 2, 35},   // 2 - 1-й пров. Перемоги
    {51.22076, 33.18776, 3, 30},   // 3 - Європейська площа (north)
    {51.22069, 33.18794, 3, 30},   // 4 - Європейська площа (south)
    {51.23117, 33.20054, 4, 25},   // 5 - Європейський університет (east)
    {51.22984, 33.19863, 4, 25},   // 6 - Європейський університет (west)
    {51.24002, 33.17508, 5, 25},   // 7 - Автокооператив №2 (north)
    {51.24012, 33.17547, 5, 25},   // 8 - Автокооператив №2 (south)
    {51.22911, 33.18566, 6, 25},   // 9 - Автостанція (east)
    {51.22878, 33.18551, 6, 25},   // 10 - Автостанція (west)
    {51.26025, 33.18866, 7, 30},   // 11 - Арматурний завод (north)
    {51.26024, 33.18844, 7, 30},   // 12 - Арматурний завод (south)
    {51.21357, 33.16199, 8, 30},   // 13 - Дитячий садок КВРЗ
    {51.25621, 33.19881, 9, 30},   // 14 - Житловий масив, АТБ (north)
    {51.25629, 33.19819, 9, 30},   // 15 - Житловий масив, АТБ (south)
    {51.26165, 33.18525, 10, 30},  // 16 - Завод "Мотордеталь" (north)
    {51.26179, 33.18476, 10, 30},  // 17 - Завод "Мотордеталь" (south)
    {51.22477, 33.18795, 11, 25},  // 18 - Залізничний вокзал (north)
    {51.22535, 33.18654, 11, 25},  // 19 - Залізничний вокзал (west)
    {51.22506, 33.18519, 11, 30},  // 20 - Залізничний вокзал (біля стоматології, south)
    {51.26807, 33.17339, 12, 35},  // 21 - Залізничний переїзд
    {51.25339, 33.20535, 13, 35},  // 22 - Кладовище жилмасив (north)
    {51.25363, 33.20467, 13, 35},  // 23 - Кладовище жилмасив (south)
    {51.21899, 33.15551, 14, 30},  // 24 - Колійно-машинна станція
    {51.22541, 33.19345, 15, 25},  // 25 - Колієпровід (east)
    {51.22561, 33.19355, 15, 25},  // 26 - Колієпровід (west)
    {51.24748, 33.17913, 16, 37},  // 27 - Ліцей №3
    {51.24888, 33.21408, 17, 28},  // 28 - Магазин "Катерина"
    {51.22024, 33.1789, 18, 30},   // 29 - Магазин "Пушинка"
    {51.21378, 33.16606, 19, 30},  // 30 - Магазин "Трудовий"
    {51.22767, 33.19621, 20, 25},  // 31 - Магазин "Фокстрот" (east)
    {51.22708, 33.19527, 20, 25},  // 32 - Магазин "Фокстрот" (west)
    {51.23304, 33.18423, 21, 25},  // 33 - Магазин 4
    {51.24698, 33.22028, 22, 25},  // 34 - Медичне училище (east)
    {51.24766, 33.22008, 22, 25},  // 35 - Медичне училище (west)
    {51.23549, 33.20574, 23, 28},  // 36 - Оптика (east)
    {51.23564, 33.20563, 23, 28},  // 37 - Оптика (west)
    {51.23903, 33.21025, 24, 30},  // 38 - Площа миру (east)
    {51.23821, 33.20886, 24, 30},  // 39 - Площа миру (west)
    {51.24854, 33.21664, 25, 30},  // 40 - Площа ринок (north)
    {51.2483, 33.21714, 25, 30},   // 41 - Площа ринок (south)
    {51.23325, 33.20302, 26, 25},  // 42 - Політехнічний коледж (east)
    {51.23308, 33.20262, 26, 25},  // 43 - Політехнічний коледж (west)
    {51.24097, 33.21911, 27, 25},  // 44 - Проспект червоної калини, Пологовий будинок (east)
    {51.2411, 33.21894, 27, 25},   // 45 - Проспект червоної калини, Пологовий будинок (west)
    {51.27767, 33.17847, 28, 35},  // 46 - СПТУ 33
    {51.24604, 33.18316, 29, 29},  // 47 - СТО сім вітрів (east)
    {51.24627, 33.18292, 29, 28},  // 48 - СТО сім вітрів (west)
    {51.21953, 33.17169, 30, 30},  // 49 - СТО січових стрільців
    {51.27559, 33.1762, 31, 30},   // 50 - Садочок Заводське (east)
    {51.27533, 33.17582, 31, 30},  // 51 - Садочок Заводське (west)
    {51.2134, 33.15883, 32, 30},   // 52 - Селище КВРЗ
    {51.22123, 33.1921, 33, 30},   // 53 - Старий ринок (east)
    {51.22113, 33.19187, 33, 30},  // 54 - Старий ринок (west)
    {51.24004, 33.17181, 34, 25},  // 55 - Стоматологічна поліклініка (north)
    {51.23992, 33.17195, 34, 25},  // 56 - Стоматологічна поліклініка (south)
    {51.25162, 33.20933, 35, 28},  // 57 - Сінний ринок, Парус (north)
    {51.25178, 33.20885, 35, 28},  // 58 - Сінний ринок, Парус (south)
    {51.24073, 33.18324, 36, 25},  // 59 - Трамвайне управляння (east)
    {51.24025, 33.18282, 36, 25},  // 60 - Трамвайне управління (north)
    {51.23962, 33.18338, 36, 24},  // 61 - Трамвайне управління (west)
    {51.25985, 33.20091, 37, 30},  // 62 - Училище №20 (east)
    {51.25986, 33.20082, 37, 30},  // 63 - Училище №20 (west)
    {51.25513, 33.2012, 38, 35},   // 64 - Фізкультурно-оздоровчий комплекс (north)
    {51.2552, 33.20084, 38, 35},   // 65 - Фізкультурно-оздоровчий комплекс (south)
    {51.21953, 33.16655, 39, 30},  // 66 - Філія харківського авіаційного заводу
    {51.25983, 33.2052, 40, 30},   // 67 - Церква жилмасив (north)
    {51.25969, 33.20534, 40, 30},  // 68 - Церква жилмасив (south)
    {51.234, 33.18438, 41, 25},    // 69 - Школа №12 (east)
    {51.23526, 33.18396, 41, 25},  // 70 - Школа №12 (west)
    {51.21438, 33.17034, 42, 30},  // 71 - Школа №14
    {51.24525, 33.21983, 43, 25},  // 72 - Школа №2, Зоряний (east)
    {51.24598, 33.21988, 43, 25},  // 73 - Школа №2, Зоряний (west)
    {51.26224, 33.20339, 44, 35},  // 74 - вул. Будівельників
    {51.22519, 33.17382, 45, 34},  // 75 - вул. Деповська (north)
    {51.22517, 33.17347, 45, 34},  // 76 - вул. Деповська (south)
    {51.24016, 33.21254, 46, 27},  // 77 - вул. Лазаревського, Фора (north)
    {51.24014, 33.21327, 46, 25},  // 78 - вул. Лазаревського, Фора (south)
    {51.22456, 33.15478, 47, 35},  // 79 - вул. Олега Куцина
    {51.21866, 33.14858, 48, 30},  // 80 - вул. Професійна
    {51.23987, 33.16893, 49, 27},  // 81 - вул. Симона Петлюри
    {51.21981, 33.17162, 50, 30},  // 82 - вул. Січових стрільців
    {51.24334, 33.18327, 51, 27},  // 83 - завод Універсал (east)
    {51.24283, 33.18314, 51, 27},  // 84 - завод Універсал (west)
    {51.22464, 33.16341, 52, 35},  // 85 - пров. Гастелло
    {51.2251, 33.17964, 53, 35},   // 86 - пров. Чехова (north)
    {51.22508, 33.17928, 53, 35},  // 87 - пров. Чехова (south)
};

#define STOPS_MAX 29
// (1) Маршрут 1
const RouteStop route0_ste[] PROGMEM = {
    {81, 0, 0},
    {56, 0, 0},
    {7, 0, 0},
    {61, 0, 0},
    {70, 0, 0},
    {33, 0, 0},
    {10, 0, 0},
    {19, 0, 0},
    {25, 1, 5},
    {31, 0, 0},
    {5, 0, 0},
    {42, 0, 0},
    {36, 0, 0},
    {38, 0, 0},
    {78, 0, 0},
    {44, 1, 5},
    {72, 0, 0},
    {34, 0, 0},
    {40, 0, 0},
    {28, 0, 0},
    {57, 0, 0},
    {22, 0, 0},
    {64, 0, 0},
    {14, 0, 0},
    {11, 0, 0},
    {16, 0, 0},
};
const RouteStop route0_ets[] PROGMEM = {
    {81, 0, 0},
    {55, 0, 0},
    {8, 0, 0},
    {60, 0, 0},
    {69, 0, 0},
    {0, 0, 0},
    {9, 0, 0},
    {18, 0, 0},
    {26, 1, 5},
    {32, 0, 0},
    {6, 0, 0},
    {43, 0, 0},
    {37, 0, 0},
    {39, 0, 0},
    {77, 0, 0},
    {45, 1, 5},
    {73, 0, 0},
    {35, 0, 0},
    {41, 0, 0},
    {0, 0, 0},
    {58, 0, 0},
    {23, 0, 0},
    {65, 0, 0},
    {15, 0, 0},
    {12, 0, 0},
    {17, 0, 0},
};

// (3) Маршрут 3
const RouteStop route1_ste[] PROGMEM = {
    {52, 0, 0},
    {13, 0, 0},
    {30, 0, 0},
    {71, 0, 0},
    {49, 0, 0},
    {29, 0, 0},
    {4, 0, 0},
    {53, 0, 0},
    {25, 1, 5},
    {31, 0, 0},
    {5, 0, 0},
    {42, 0, 0},
    {78, 0, 0},
    {44, 1, 5},
    {72, 0, 0},
    {34, 0, 0},
    {40, 0, 0},
    {28, 0, 0},
    {57, 0, 0},
    {22, 0, 0},
    {64, 0, 0},
    {14, 0, 0},
    {62, 0, 0},
    {74, 0, 0},
};
const RouteStop route1_ets[] PROGMEM = {
    {52, 0, 0},
    {13, 0, 0},
    {30, 0, 0},
    {71, 0, 0},
    {49, 0, 0},
    {29, 0, 0},
    {3, 0, 0},
    {54, 0, 0},
    {26, 1, 5},
    {32, 0, 0},
    {6, 0, 0},
    {43, 0, 0},
    {77, 0, 0},
    {45, 1, 5},
    {73, 0, 0},
    {35, 0, 0},
    {41, 0, 0},
    {0, 0, 0},
    {58, 0, 0},
    {23, 0, 0},
    {65, 0, 0},
    {15, 0, 0},
    {63, 0, 0},
    {74, 0, 0},
};

// (8) Маршрут 8
const RouteStop route2_ste[] PROGMEM = {
    {80, 0, 0},
    {24, 0, 0},
    {66, 0, 0},
    {82, 0, 0},
    {29, 0, 0},
    {4, 0, 0},
    {53, 0, 0},
    {25, 1, 5},
    {31, 0, 0},
    {5, 0, 0},
    {42, 0, 0},
    {36, 0, 0},
    {38, 0, 0},
    {78, 0, 0},
    {44, 1, 5},
    {72, 0, 0},
    {34, 0, 0},
    {40, 0, 0},
    {28, 0, 0},
    {57, 0, 0},
    {22, 0, 0},
    {64, 0, 0},
    {14, 0, 0},
    {62, 0, 0},
    {74, 0, 0},
};
const RouteStop route2_ets[] PROGMEM = {
    {80, 0, 0},
    {24, 0, 0},
    {66, 0, 0},
    {82, 0, 0},
    {29, 0, 0},
    {3, 0, 0},
    {54, 0, 0},
    {26, 1, 5},
    {32, 0, 0},
    {32, 0, 0},
    {43, 0, 0},
    {37, 0, 0},
    {39, 0, 0},
    {77, 0, 0},
    {45, 1, 5},
    {73, 0, 0},
    {35, 0, 0},
    {41, 0, 0},
    {0, 0, 0},
    {58, 0, 0},
    {23, 0, 0},
    {65, 0, 0},
    {15, 0, 0},
    {63, 0, 0},
    {74, 0, 0},
};

// (14) Маршрут 14
const RouteStop route3_ste[] PROGMEM = {
    {27, 0, 0},
    {48, 0, 0},
    {84, 0, 0},
    {61, 0, 0},
    {70, 0, 0},
    {33, 0, 0},
    {10, 0, 0},
    {19, 0, 0},
    {25, 1, 5},
    {31, 0, 0},
    {5, 0, 0},
    {42, 0, 0},
    {36, 0, 0},
    {38, 0, 0},
    {78, 0, 0},
    {44, 1, 5},
    {72, 0, 0},
    {34, 0, 0},
    {40, 0, 0},
    {28, 0, 0},
    {57, 0, 0},
    {22, 0, 0},
    {64, 0, 0},
    {14, 0, 0},
    {11, 0, 0},
    {16, 0, 0},
    {21, 0, 0},
    {50, 0, 0},
    {46, 0, 0},
};
const RouteStop route3_ets[] PROGMEM = {
    {27, 0, 0},
    {47, 0, 0},
    {83, 0, 0},
    {59, 0, 0},
    {69, 0, 0},
    {0, 0, 0},
    {9, 0, 0},
    {18, 0, 0},
    {26, 1, 5},
    {32, 0, 0},
    {6, 0, 0},
    {43, 0, 0},
    {37, 0, 0},
    {39, 0, 0},
    {77, 0, 0},
    {45, 1, 5},
    {73, 0, 0},
    {35, 0, 0},
    {41, 0, 0},
    {0, 0, 0},
    {58, 0, 0},
    {23, 0, 0},
    {65, 0, 0},
    {15, 0, 0},
    {12, 0, 0},
    {17, 0, 0},
    {21, 0, 0},
    {51, 0, 0},
    {46, 0, 0},
};

// (16) Маршрут 16
const RouteStop route4_ste[] PROGMEM = {
    {79, 0, 0},
    {2, 0, 0},
    {85, 0, 0},
    {1, 0, 0},
    {76, 0, 0},
    {87, 0, 0},
    {20, 0, 0},
    {25, 1, 5},
    {31, 0, 0},
    {5, 0, 0},
    {42, 0, 0},
    {36, 0, 0},
    {38, 0, 0},
    {78, 0, 0},
    {44, 1, 5},
    {72, 0, 0},
    {34, 0, 0},
    {40, 0, 0},
    {28, 0, 0},
    {57, 0, 0},
    {22, 0, 0},
    {64, 0, 0},
    {14, 0, 0},
    {62, 0, 0},
    {67, 0, 0},
};
const RouteStop route4_ets[] PROGMEM = {
    {79, 0, 0},
    {2, 0, 0},
    {85, 0, 0},
    {1, 0, 0},
    {75, 0, 0},
    {86, 0, 0},
    {18, 0, 0},
    {26, 1, 5},
    {32, 0, 0},
    {6, 0, 0},
    {43, 0, 0},
    {37, 0, 0},
    {39, 0, 0},
    {77, 0, 0},
    {45, 1, 5},
    {73, 0, 0},
    {35, 0, 0},
    {41, 0, 0},
    {0, 0, 0},
    {58, 0, 0},
    {58, 0, 0},
    {65, 0, 0},
    {15, 0, 0},
    {63, 0, 0},
    {68, 0, 0},
};

#define ROUTES_NUM 5
const Route routes[] PROGMEM = {
    {1, 26, route0_ste, route0_ets},   // 0 - (1) Маршрут 1
    {3, 24, route1_ste, route1_ets},   // 1 - (3) Маршрут 3
    {8, 25, route2_ste, route2_ets},   // 2 - (8) Маршрут 8
    {14, 29, route3_ste, route3_ets},  // 3 - (14) Маршрут 14
    {16, 25, route4_ste, route4_ets},  // 4 - (16) Маршрут 16
};

class Coordinates {
private:
    uint8_t currentRouteIndex = 0;
    Route currentRoute;

    float getCoord(uint16_t stopId, uint8_t latORlong) {
        Stop temp;
        memcpy_P(&temp, &stops[stopId], sizeof(Stop));

        if (latORlong == 0) {
            return temp.lat;
        } else {
            return temp.lon;
        }
    }

public:
    Coordinates() {
        currentRouteIndex = EEPROM.read(0);    // read saved index
        if (currentRouteIndex > ROUTES_NUM) {  // check if saved index exist
            currentRouteIndex = 0;             // if it doesn't, then use first route in the list
        }

        memcpy_P(&currentRoute, &routes[currentRouteIndex], sizeof(Route));
    }

    void nextLine() {
        currentRouteIndex == ROUTES_NUM - 1 ? currentRouteIndex = 0 : currentRouteIndex++;
        memcpy_P(&currentRoute, &routes[currentRouteIndex], sizeof(Route));
    }

    void prevLine() {
        currentRouteIndex == 0 ? currentRouteIndex = ROUTES_NUM - 1 : currentRouteIndex--;
        memcpy_P(&currentRoute, &routes[currentRouteIndex], sizeof(Route));
    }

    void rememberRoute() {
        EEPROM.update(0, currentRouteIndex);
    }

    uint8_t currentRouteDispNum() {
        return currentRoute.dispNum;
    }

    float getLat(uint8_t routeStopIndex, bool startToEnd) {
        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        return getCoord(routeStop.stopIndex, 0);
    }

    float getLng(uint8_t routeStopIndex, bool startToEnd) {
        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        return getCoord(routeStop.stopIndex, 1);
    }

    uint8_t getStopAudio(uint8_t routeStopIndex, bool startToEnd) {
        if (routeStopIndex >= getStopsNum()) return 0;

        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        Stop stop;
        memcpy_P(&stop, &stops[routeStop.stopIndex], sizeof(Stop));

        return stop.audioNum;
    }

    uint8_t getStopRadius(uint8_t routeStopIndex, bool startToEnd) {
        if (routeStopIndex >= getStopsNum()) return 0;

        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        Stop stop;
        memcpy_P(&stop, &stops[routeStop.stopIndex], sizeof(Stop));

        return stop.radius;
    }

    uint8_t getRouteStopAfterAudioNumber(uint8_t routeStopIndex, bool startToEnd) {
        if (routeStopIndex >= getStopsNum()) return 0;

        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        return routeStop.afterAudio;
    }

    uint16_t getRouteStopAfterAudioDelay(uint8_t routeStopIndex, bool startToEnd) {
        if (routeStopIndex >= getStopsNum()) return 0;

        RouteStop routeStop;

        if (startToEnd) {
            memcpy_P(&routeStop, &currentRoute.steStops[routeStopIndex], sizeof(RouteStop));
        } else {
            memcpy_P(&routeStop, &currentRoute.etsStops[routeStopIndex], sizeof(RouteStop));
        }

        return routeStop.afterDelay;
    }

    uint8_t getStopsNum() {
        return currentRoute.stopsNum;
    }
};

#endif  // COORDINATES_H