#pragma once


class GameModuleInterface
{
public:
  virtual ~GameModuleInterface() {};

  virtual void Update(float delta) = 0;
};
