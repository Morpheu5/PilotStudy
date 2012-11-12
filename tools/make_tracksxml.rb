#!/usr/bin/env ruby

if ARGV.length < 1
  puts "Usage: make_tracksxml.rb <path_to_clips>"
  exit -1
end

if !File.directory?(ARGV[0])
  puts "#{ARGV[0]} is not a directory"
  exit -2
end

filenames = Dir.glob("#{ARGV[0]}/*.wav")
i = 1

puts '<?xml version="1.0" ?>' + "\n" + '<root>'

filenames.each do |f|
  puts "\t" + '<track id="' + i.to_s + '" file="' + File.basename(f, '.wav') + '" />'
  i += 1
end

puts '</root>'