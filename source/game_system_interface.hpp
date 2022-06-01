#pragma once


class GameSystemInterface
{
public:
  virtual ~GameSystemInterface() {};

  virtual void StartUp() = 0;
  virtual void ShutDown() = 0;
  virtual bool IsWorking() = 0;
};
