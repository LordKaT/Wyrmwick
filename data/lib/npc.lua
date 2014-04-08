npc = {}

npc.list = {}

function npc.register(person)
	if person.id == nil then
		error("npc doesn't have an ID", 2)
	end
	npc.list[person.id] = person
end
