
count = 1

Dir.glob('*.png').each do |f|
  open(f, "rb") do |f1|
    open("FILE000%03d.png" % count, "wb") do |f2|
      f2.write(f1.read)
    end
  end
  count += 1
end

exit
