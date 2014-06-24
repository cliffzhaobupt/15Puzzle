class Ranking < ActiveRecord::Base
  validates :name, presence: {message: 'must have a name'}
  validates :time, presence: {message: 'must have a time'}
end
