DialogTree = {}
DialogTree.wait = { action = "wait" }
DialogTree.bye = { action = "bye" }
DialogTree.jump = function(ch) return { action = "jump", where = ch } end

function DialogTree.make(lst)
	local nt = {}
	for k, v in pairs(DialogTree) do nt[k] = v end
	nt.nodes = {}
	
	local function unpackNode(node)
		res = { choices = {}, responses = {} }
		res.prompt = table.remove(node, 1)
		for i, v in ipairs(node) do
			table.insert(res.choices, table.remove(v, 1))
			res.responses[i] = v
		end
		return res
	end
	
	for k, v in pairs(lst) do nt.nodes[k] = unpackNode(v) end
	nt:reset()
	return nt
end

function DialogTree:attach(npc)
	npc.dialog = self
	npc.onActivate = function(thisnpc)
		local convo = game.startTalking()
		thisnpc.dialog:reset()
		thisnpc.dialog:run(convo)
		convo.stop()
	end
end

function DialogTree:reset()
	self:jumpTo("main")
	self.finished = false
end

function DialogTree:jumpTo(dst)
	if self.nodes[dst] == nil then
		error("can't jump to \"" .. dst .. "\": no such choice", 2)
	end
	self.current, self.currentName = self.nodes[dst], dst
end

function DialogTree:doAction(act, conv)
	local function single(act, conv)
		if type(act) == "table" then
			if act.action == "bye" then
				self.finished = true
			elseif act.action == "wait" then
				conv.wait()
			elseif act.action == "jump" then
				self:jumpTo(act.where)
			end
		elseif type(act) == "string" then
			conv.say(act)
			conv.wait()
		elseif type(act) == "function" then
			act(conv)
		end
	end
	
	if type(act) ~= "table" or act.type ~= nil then
		single(act, conv)
		return
	end
	
	for i = 1, #act do single(act[i], conv) end
end

function DialogTree:run(conv)
	conv.say(self.current.prompt)
	if self.finished then return end
	
	conv:resetChoices()
	for i = 1, #self.current.choices do
		conv.addChoice(self.current.choices[i])
	end
	
	local ch = conv.offerChoice()
	if ch == -1 then return end
	
	self:doAction(self.current.responses[ch + 1], conv)
	if self.finished then return end
	return self:run(conv)
end
