
function dbg_print(o)
  print("=== Info about " .. tostring(o) .. " ===")
  print("    Type:" .. type(o))
  if type(o) == "table" then
    table.foreach(o, function(k,v)
      print("    " .. tostring(k) .. " -> " .. tostring(v))
    end)
    mt = getmetatable(o)

    if mt then
      print()
      print("  metatable:")
      table.foreach(mt, function(k,v)
        print("    " .. tostring(k) .. " -> " .. tostring(v))
      end)
    end
  end

end


