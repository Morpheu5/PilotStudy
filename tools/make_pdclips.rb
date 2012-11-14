#!/usr/bin/env ruby

if ARGV.length < 1
  puts "Usage: make_pdclips.rb <path_to_clips>"
  exit -1
end

if !File.directory?(ARGV[0])
  puts "#{ARGV[0]} is not a directory"
  exit -2
end

filenames = Dir.glob("#{ARGV[0]}/*.wav")
i = 1
y = 10
l = 3

puts "#N canvas 0 0 900 900 10;"
puts "#X obj 10 10 soundfiler;"
puts "#X obj 10 40 loadbang;"

filenames.each do |f|
  name = File.basename(f, ".wav")
  puts "#X obj 500 #{y} table #{name};"
  puts "#X msg 150 #{y} read -resize #{f} #{name};"
  y += 30
  i += 1
end

filenames.each do |f|
  puts "#X connect #{l} 0 0 0;"
  puts "#X connect 1 0 #{l} 0;"
  l += 2
end