#include "TileMap.h"



TileMap::TileMap(float _width, float _height, Sprite *_sprite, float _frameWidth, float _frameHeight)
{
	width = _width;
	height = _height;
	sprite = _sprite;

	frameWidth = _frameWidth;
	frameHeight = _frameHeight;

	spritePerRow = sprite->GetWidth() / frameWidth;

	rows = height / frameHeight;
	cols = width / frameWidth;
	matrix = new int*[rows];
	for (int i = 0; i < rows; i++)
		matrix[i] = new int[cols];
}


TileMap::~TileMap()
{
	if (matrix)
	{
		for (int i = 0; i < rows; i++)
		{
			delete matrix[i];
		}
		delete matrix;
		matrix = nullptr;
	}

	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void TileMap::LoadListTileFromFile(const char *file)
{
	fstream pFile;
	pFile.open(file, fstream::in);

	string lineString;
	int *posSpace = new int[cols];
	string subString;
	int i = 0;

	while (pFile.good())
	{
		getline(pFile, lineString);
		if (lineString.find("END") != string::npos)
			break;

		for (int j = 0; j < cols; j++)
		{
			if (j == 0)
			{
				posSpace[0] = lineString.find(" ", 0);
				subString = lineString.substr(0, posSpace[0]);
				int id = atoi(subString.c_str());
				matrix[i][j] = id;
			}
			else
			{
				posSpace[j] = lineString.find(" ", posSpace[j - 1] + 1);
				subString = lineString.substr(posSpace[j - 1] + 1, posSpace[j] - (posSpace[j - 1] + 1));
				int id = atoi(subString.c_str());
				matrix[i][j] = id;
			}
		}
		i++;
	}
}

void TileMap::Draw(ViewPort *viewPort)
{
	RECT rect;
	D3DXVECTOR3 pos;

	D3DXVECTOR3 cameraPosition = viewPort->GetCameraPosition();
	int rowStart = abs(480 - cameraPosition.y) / frameHeight - 2 < 0 ? 0 : abs(480 - cameraPosition.y) / frameHeight - 2;
	int rowEnd = (cameraPosition.y / frameHeight) > rows ? rows : (cameraPosition.y / frameHeight);
	int start = rows - rowEnd;
	int end = rows - rowStart;
	int colStart = cameraPosition.x / frameWidth;
	int colEnd = (cameraPosition.x / frameWidth + 640 / frameWidth + 1) > cols ? cols : (cameraPosition.x / frameHeight + 640 / frameWidth + 1);

	//Ve tat ca cac tile nam trong camera
	for (int i = start; i < end; i++)
	{
		for (int j = colStart; j < colEnd; j++)
		{
			rect.left = (matrix[i][j] % spritePerRow) * frameWidth;
			rect.top = (matrix[i][j] / spritePerRow) * frameHeight;
			rect.right = rect.left + frameWidth;
			rect.bottom = rect.top + frameHeight;
			pos.x = j * frameWidth;
			pos.y = (rows - i) * frameHeight;
			pos.z = 0;
			pos = viewPort->SetPositionInViewPort(pos);
			sprite->Render(pos, rect);
		}
	}
}
