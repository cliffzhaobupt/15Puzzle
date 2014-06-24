# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)

Ranking.destroy_all
100.step(1500, 100) do |time|
  Ranking.create(
    name: (time % 200 == 0) ? 'tom' : 'cliff',
    time: time
    )
end