#!/usr/bin/env ruby

if ARGV.length < 1
  puts "Usage: make_loopsxml.rb <path_to_clips>"
  exit -1
end

if !File.directory?(ARGV[0])
  puts "#{ARGV[0]} is not a directory"
  exit -2
end

filenames = Dir.glob("#{ARGV[0]}/*.wav")
i = 1

puts '<?xml version="1.0" ?>' + "\n" + '<root>' + "\n\t" + '<loops>'

filenames.each do |f|
  puts "\t\t" + '<loop name="' + File.basename(f, '.wav') + '" />'
  i += 1
end

puts "\t</loops>\n</root>"