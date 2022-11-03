modded class BUISOptic extends ItemOptics {
	override bool CanPutAsAttachment(EntityAI parent) {
		if (parent.IsKindOf("HK416_Base_CFTools")) {
			return true;
		}

        return super.CanPutAsAttachment(parent);		
	}
};

modded class M4_CarryHandleOptic extends ItemOptics {
	override bool CanPutAsAttachment(EntityAI parent) {
		if (parent.IsKindOf("HK416_Base_CFTools")) {
			return true;
		}

        return super.CanPutAsAttachment(parent);		
	}
};