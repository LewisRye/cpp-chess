#include "ChessGraphicsSystem.hpp"
#include "ChessTexture.hpp"
#include "ClickEventHandler.hpp"
#include "GraphicsConstants.hpp"
#include "../board.h"
#include <SDL_image.h>


// --------------------------------------------------------------------------------------------------------------------
ChessGraphicsSystem::ChessGraphicsSystem() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw ("SDL could not initialize! SDL Error: " + std::string(SDL_GetError()) + "\n");
	}

    // Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    // Create window
    mWindow = SDL_CreateWindow(
        "HackSussex Chess",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if(!mWindow) {
        throw ("Window could not be created! SDL Error: " +  std::string(SDL_GetError()) + "\n");
    }

    //Create renderer for window
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!mRenderer) {
        throw ("Renderer could not be created! SDL Error:" + std::string(SDL_GetError()) + "\n");
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        throw ("SDL_image could not initialize! SDL_image Error:" + std::string(IMG_GetError()) + "\n");
    }

    mClickEventHandler = new ClickEventHandler();
    mChessBoardTexture = new ChessTexture(mRenderer);
    mChessPiecesSpriteSheetTexture = new ChessTexture(mRenderer);

    LoadAllMedia();
}

// --------------------------------------------------------------------------------------------------------------------
ChessGraphicsSystem::~ChessGraphicsSystem() {
	// Destroy window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	mRenderer = nullptr;
    
    delete mChessBoardTexture;
    delete mChessPiecesSpriteSheetTexture;
    mChessBoardTexture = nullptr;
    mChessPiecesSpriteSheetTexture = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

// --------------------------------------------------------------------------------------------------------------------
SDL_Point ChessGraphicsSystem::ConvertGridCoordinatesToSDLCoordinates(int x, int y) {
	auto p = SDL_Point();
    p.x = OFFSET_FROM_EDGE_OF_BOARD + x*CHESS_TILE_SIZE;
    p.y = OFFSET_FROM_EDGE_OF_BOARD + y*CHESS_TILE_SIZE;
    return p;
}

// --------------------------------------------------------------------------------------------------------------------
int ChessGraphicsSystem::GetChessFigureTextureBoundingRectIndex(PieceColour pieceColour, PieceType pieceType) {
    // In mChessPiecesTexturesBoundingRects, 0-5 are black rook, knight, bishop, queen, king, pawn respectively
    // 6 - 11 are their counterparts for white

    int dimPieceColour = (pieceColour == PieceColour::kBlack) ? 0 : 1;
    int dimPieceType = 0;
    switch (pieceType) {
        case PieceType::kRook:
            dimPieceType = 0;
            break;
        case PieceType::kKnight:
            dimPieceType = 1;
            break;
        case PieceType::kBishop:
            dimPieceType = 2;
            break;
        case PieceType::kQueen:
            dimPieceType = 3;
            break;
        case PieceType::kKing:
            dimPieceType = 4;
            break;
        case PieceType::kPawn:
            dimPieceType = 5;
            break;
        default:
            break;
    }
    return dimPieceColour*6 + dimPieceType;
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::LoadAllMedia() {

	// Load pieces textures
	if (!mChessPiecesSpriteSheetTexture->LoadTextureFromFile(CHESS_PIECES_FILEPATH)) {
		throw("Failed to load chess pieces sprite sheet texture!\n");
	}

    for (int i = 0; i < 12; i++) {
        mChessPiecesTexturesBoundingRects[i].x = (i % 6) * CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].y = (i < 6) ? 0 : CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].w = CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].h = CHESS_TILE_SIZE;
    }

	// Load board texture
	if (!mChessBoardTexture->LoadTextureFromFile(CHESS_BOARD_FILEPATH)) {
		throw("Failed to load board texture image!\n");
	}
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::Render(Board *board) {
    // Clear screen
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    RenderBoardState(board);

    // Update screen
    SDL_RenderPresent(mRenderer);
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::RenderBoardState(Board *board) {
    // Render board and pieces
    mChessBoardTexture->Render(0, 0);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Square *currSquare = board->getSquare(i, j);
            if (!currSquare->isEmpty()) {
                auto currPiece = currSquare->getPiece();
                mChessPiecesSpriteSheetTexture->Render(
                    OFFSET_FROM_EDGE_OF_BOARD + i * CHESS_TILE_SIZE,
                    OFFSET_FROM_EDGE_OF_BOARD + j * CHESS_TILE_SIZE,
                    &mChessPiecesTexturesBoundingRects[
                        GetChessFigureTextureBoundingRectIndex(
                            currPiece->getColour(),
                            currPiece->getType()
                        )
                    ]
                );
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::RunChessGame(Board *board) {
    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e))
        {
            // User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
            }

            //Handle click events
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int clickX, clickY;
                    SDL_GetMouseState(&clickX, &clickY);
                    mClickEventHandler->HandleClickEvent(clickX, clickY);
                }
            }
        }

        Render(board);
    }
}

// --------------------------------------------------------------------------------------------------------------------