#pragma once


class EnviromentSystemInterface
{
public:
  virtual ~EnviromentSystemInterface() {};

  virtual void Init() = 0;
  virtual void Deinit() = 0;
  virtual bool IsInitialized() = 0;
};
