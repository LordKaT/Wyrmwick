DialogTree = {}
DialogTree.bye = { action = "bye" }
DialogTree.jump = function(ch) return { action = "jump", where = ch } end

function DialogTree.make(lst)
	local nt = {}
	for k, v in pairs(DialogTree) do nt[k] = v end
	nt.nodes = lst
	nt:reset()
	return nt
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
				conv.wait()
				self.finished = true
			elseif act.action == "goto" then
				self:jumpTo(act.where)
			end
		elseif type(act) == "string" then
			conv.say(act)
		elseif type(act) == "function" then
			act(conv)
		else
	end
	
	if type(act) ~= "table" or act.type ~= nil then
		single(act, conv)
		return
	end
	
	for i = 1, #act do single(act[i], conv) end
		single(act, conv)
	end
end

function DialogTree:run(conv)
	self:doAction(self.current.prompt, conv)
	if self.finished then return end
	
	conv:resetChoices()
	for i = 2, #self.current-1 do
		conv:addChoice(self.current.choices[i])
	end
	local ch = conv:offerChoice()
	
	if ch == -1 then return end
	self:doAction(self.current.responses[ch])
	if self.finished then return end
	return self:run(conv)
end
