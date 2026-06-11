#include "ANetGameMode.h"
#include "ANetBaseCharacter.h"

ANetGameMode::ANetGameMode()
{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();
}
