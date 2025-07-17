#include "features/trigger.hpp"

bool Trigger::Update(ProcessHelper& memory) {
	if (Globals::triggerEnabled) {
		// read entity index
		uint32_t entityIndex;
		memory.ReadMemory(Globals::client_dll + Offsets::client_dll::C_BaseEntity::C_BasePlayerPawn::m_iIDEntIndex, &entityIndex, sizeof(entityIndex));
		if (entityIndex == 0) return false;


	}
	return true;
}
