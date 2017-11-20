#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>


typedef unsigned long long u64;


///Math constants
const double kEpsilon = 0.000001;
const u64 kDebruijn64 = 0x03f79d71b4cb0a89;


///Screen
const int kScreenWidth = 1200;
const int kScreenHeight = 800;
const int kScreenDiag = sqrt(kScreenWidth * kScreenWidth + kScreenHeight * kScreenHeight);
const int kFrameRateLimit = 200;
const int kAntialiasingLevel = 8;
const std::string kScreenTitle = "Foompello";
const int kBoardWidth = 8;
const int kBoardHeight = 8;
const int kSquareSizeInPixels = kScreenHeight * 3 / 4 / 8;
const int kXMargin = (kScreenWidth - kBoardWidth * kSquareSizeInPixels) / 2;
const int kYMargin = (kScreenHeight - kBoardHeight * kSquareSizeInPixels) / 2;
const int kDiskRadius = kSquareSizeInPixels * 0.9 / 2;
const double kInfinity = 1000000.0;
const double kNegativeInfinity = -kInfinity;
//const int kMinimaxDepthMedium = 6;
//const int kMinimaxDepthHard = 10;
//const int kMinimaxDepthVeryHard = 12;
//const sf::Color BACKGROUND_COLOR = sf::Color(200, 200, 200);
//const sf::Color BACKGROUND_COLOR = sf::Color(255, 255, 255);


///Colors
const sf::Color kMainMenuStateBackgroundColor = sf::Color(200, 200, 200);
const sf::Color kPlayStateBackgroundColor = sf::Color(0, 0, 0);
const sf::Color kGameOverStateBackgroundColor = sf::Color(200, 200, 200);


//const double OFFSET_X = 0;
//const double OFFSET_Y = INFO_RECT_SHAPE_HEIGHT;

///Tile symbols
const char WHITE_TILE = '0';


///Resource paths
const std::string kFontsPath = "resources/fonts/";
const std::string kTexturesPath = "resources/textures/";
const std::string kSoundsPath = "resources/sounds/";
const std::string kMusicPath = "resources/music/";

///Text size
const int kGameOverTextSize = 60;
const int kScoreTextSize = 40;
const int kPressAnyKeyTextSize = 20;
const int kMenuTextSize = 20;
const int kScoreFontSize = kSquareSizeInPixels * 0.4;



//const sf::Vector2f INFO_RECT_SHAPE_INITIAL_POSITION = sf::Vector2f(0, 0);
//const sf::Vector2f INFO_TEXT_INITIAL_POSITION = sf::Vector2f(10, 10);

//const sf::Vector2f VIEW_INITIAL_CENTER = sf::Vector2f(kScreenWidth / 2, kScreenHeight / 2);
//const sf::Vector2f VIEW_INITIAL_SIZE = sf::Vector2f(kScreenWidth, kScreenHeight);


const int kMenuButtonWidth = 200;
const int kMenuButtonHeight = 100;
const sf::Vector2f kMenuButtonSize = sf::Vector2f(kMenuButtonWidth, kMenuButtonHeight);
const sf::Color kMenuButtonColor = sf::Color(255, 255, 255);
const double kMenuButtonOutlineThickness = 5;
const sf::Color kMenuButtonOutlineColor = sf::Color(0, 0, 0);

const int kMenuNewGameButtonPositionX = (kScreenWidth - kMenuButtonWidth) / 2;
const int kMenuNewGameButtonPositionY = 300;
const sf::Vector2f kMenuNewGameButtonPosition = sf::Vector2f(kMenuNewGameButtonPositionX, kMenuNewGameButtonPositionY);

const int kMenuOptionsButtonPositionX = kMenuNewGameButtonPositionX - kMenuButtonWidth - 2 * kMenuButtonOutlineThickness;
const int kMenuOptionsButtonPositionY = kMenuNewGameButtonPositionY + kMenuButtonHeight + 2 * kMenuButtonOutlineThickness;
const sf::Vector2f kMenuOptionsButtonPosition = sf::Vector2f(kMenuOptionsButtonPositionX, kMenuOptionsButtonPositionY);

const int kMenuExitButtonPositionX = kMenuOptionsButtonPositionX - kMenuButtonWidth - 2 * kMenuButtonOutlineThickness;
const int kMenuExitButtonPositionY = kMenuOptionsButtonPositionY + kMenuButtonHeight + 2 * kMenuButtonOutlineThickness;
const sf::Vector2f kMenuExitButtonPosition = sf::Vector2f(kMenuExitButtonPositionX, kMenuExitButtonPositionY);



const sf::Color kMenuTextColor = sf::Color(0, 0, 0);
const int kMenuCharacterSize = 20;

const sf::Color kMenuButtonDefaultColor = sf::Color(255, 255, 255);
const sf::Color kMenuButtonOnSelectColor = sf::Color(0, 150, 0);


///Options state
const int kOptionsWindowWidth = 600;
const int kOptionsWindowHeight = 400;
const int kOptionsStateTextSize = 20;

///Sound
const int kPlayStateVolumeLevel = 3;


const std::unordered_map<int, std::string> int_color_to_string({{1, "BLACK"}, {-1, "WHITE"}});

const std::string kBoardCoordinates[64] = {
                                          "H8", "G8", "F8", "E8", "D8", "C8", "B8", "A8",
                                          "H7", "G7", "F7", "E7", "D7", "C7", "B7", "A7",
                                          "H6", "G6", "F6", "E6", "D6", "C6", "B6", "A6",
                                          "H5", "G5", "F5", "E5", "D5", "C5", "B5", "A5",
                                          "H4", "G4", "F4", "E4", "D4", "C4", "B4", "A4",
                                          "H3", "G3", "F3", "E3", "D3", "C3", "B3", "A3",
                                          "H2", "G2", "F2", "E2", "D2", "C2", "B2", "A2",
                                          "H1", "G1", "F1", "E1", "D1", "C1", "B1", "A1",
                                          };


const int kIndex64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

//GameIsOver window
const int kGameIsOverWindowWidth = 200;
const int kGameIsOverWindowHeight = 150;
const sf::VideoMode kGameIsOverWindowVideoMode = sf::VideoMode(kGameIsOverWindowWidth, kGameIsOverWindowHeight);
const sf::Vector2f kGameIsOverWindowSize = sf::Vector2f(kGameIsOverWindowWidth, kGameIsOverWindowHeight);
const int kGameIsOverWindowPositionX = 100;
const int kGameIsOverWindowPositionY = 50;
const sf::Vector2f kGameIsOverWindowPosition = sf::Vector2f(kGameIsOverWindowPositionX, kGameIsOverWindowPositionY);
const int kGameIsOverWindowFontSize = 20;
const std::string kGameIsOverWindowTitle = "Final score";
//const sf::Color kGameIsOverWindowFillColor = sf::Color(0, 200, 255);
const sf::Color kGameIsOverWindowFillColor = sf::Color(250, 120, 0);

//GameIsOverOk button
const int kGameIsOverOkButtonWidth = 40;
const int kGameIsOverOkButtonHeight = 30;
const sf::Vector2f kGameIsOverOkButtonSize = sf::Vector2f(kGameIsOverOkButtonWidth, kGameIsOverOkButtonHeight);
const int kGameIsOverOkButtonPositionX = (kGameIsOverWindowWidth - kGameIsOverOkButtonWidth) / 2;
const int kGameIsOverOkButtonPositionY = (kGameIsOverWindowHeight - kGameIsOverOkButtonHeight) * 0.75;
const sf::Vector2f kGameIsOverOkButtonPosition = sf::Vector2f(kGameIsOverOkButtonPositionX, kGameIsOverOkButtonPositionY);
const sf::Color kGameIsOverOkButtonColor = sf::Color(250, 250, 250);
const int kGameIsOverOkButtonOutlineThickness = 1;
const sf::Color kGameIsOverOkButtonOutlineColor = sf::Color(0, 0, 0);
const std::string kGameIsOverOkButtonString = "Ok";
const int kGameIsOverOkButtonCharacterSize = 17;
const sf::Color kGameIsOverOkButtonTextColor = sf::Color(0, 0, 0);
const sf::Color kGameIsOverWindowOkButtonOnSelectColor = sf::Color(kMenuButtonOnSelectColor);
//const sf::Color kGameIsOverWindowOkButtonOnSelectColor = sf::Color(250, 120, 10);
//const sf::Color kGameIsOverWindowOkButtonOnSelectColor = sf::Color(255, 255, 255);
const sf::Color kGameIsOverWindowOkButtonDefaultColor = sf::Color(255, 255, 255);

const int kGameIsOverWindowFinalScoreTextPositionX = 20;
const int kGameIsOverWindowFinalScoreTextPositionY = 20;
const sf::Vector2f kGameIsOverWindowFinalScoreTextPosition = sf::Vector2f(kGameIsOverWindowFinalScoreTextPositionX, kGameIsOverWindowFinalScoreTextPositionY);
const sf::Color kGameIsOverWindowFinalScoreTextColor = sf::Color(0, 0, 0);
const int kGameIsOverWindowFinalScoreTextCharacterSize = 20;


const int kBoardHistorySize = 1000;
const int kMinimaxDepthMedium = 6;
const int kMinimaxDepthHard = 10;
const int kTimePerMoveMedium = 1000;//in milliseconds
const int kTimePerMoveHard = 3000;//in milliseconds
//const int ZOBRIST_TABLE_SIZE = 50000;

const int kMaxMoveNumber = 81;

#endif // CONFIG_H

