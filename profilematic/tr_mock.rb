#!/usr/bin/ruby
#
# Used to create test translation to check that each string is translated.
#
# Usage example: ./tr_mock.rb i18n.source/tr_en.ts "TEST " > i18n.source/tr_ts.ts
# ./tr_release.sh
# LANG=tz ./profilematic

require 'nokogiri'

if ARGV.count < 2
	print "Usage: tr_mock input_file prependToTranslation"
	exit 1
end

input_file, prepend = ARGV[0..1]

doc = Nokogiri::XML.parse(open(input_file))
# puts "Go noko " + doc.to_s

sources_s = doc.search('source').map(&:text)
translations = doc.search('translation')

sources_s.each_with_index { |source_s, index|
	translation = translations[index]
	translation.content = prepend + source_s
	translation.remove_attribute('type') # Remove "unfinished" attribute
}

puts doc.to_s

