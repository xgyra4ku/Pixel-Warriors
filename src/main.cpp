#include "../include/Engine.hpp"
#include "../include/cInfoConsole.hpp"

int main()
{
	cInfoConsole cCmdInfo;
	cInfoConsole::enableANSI();
	cCmdInfo.info("Starting Pixel Warriors");
    Engine game;
    game.vRun();
    cCmdInfo.info("Exiting Pixel Warriors");
	return 0;
}