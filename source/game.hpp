#pragma once

#define DllExport   __declspec( dllexport )


class DllExport Game
{
public:
  Game();
  ~Game();

  int Run();
};
