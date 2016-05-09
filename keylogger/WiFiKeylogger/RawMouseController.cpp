// For future mouse logging support

#include "RawMouseController.h"

void RawMouseController::OnMouseMove(MOUSEINFO *mi)
{
  Serial.print("dx=");
  Serial.print(mi->dX, DEC);
  Serial.print(" dy=");
  Serial.println(mi->dY, DEC);
};
void RawMouseController::OnLeftButtonUp  (MOUSEINFO *mi)
{
  Serial.println("L Butt Up");
};
void RawMouseController::OnLeftButtonDown (MOUSEINFO *mi)
{
  Serial.println("L Butt Dn");
};
void RawMouseController::OnRightButtonUp  (MOUSEINFO *mi)
{
  Serial.println("R Butt Up");
};
void RawMouseController::OnRightButtonDown  (MOUSEINFO *mi)
{
  Serial.println("R Butt Dn");
};
void RawMouseController::OnMiddleButtonUp (MOUSEINFO *mi)
{
  Serial.println("M Butt Up");
};
void RawMouseController::OnMiddleButtonDown (MOUSEINFO *mi)
{
  Serial.println("M Butt Dn");
};
