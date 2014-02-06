-- returns:
--		Name, Description, ItemType, WeaponType, EquipType
function item_init()
	local name = "Dummy"
	local desc = "this is a dummy item description. Read all about it."
	local itemType = ITEM_NONE
	local weaponType = WEAPON_NONE
	local equipType = EQ_NONE
	return name, desc, itemType, weaponType, equipType
end
