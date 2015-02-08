class CreateSearches < ActiveRecord::Migration
  def change
    create_table :searches do |t|
      t.string :userIDs
      t.string :minPrice
      t.string :maxPrice
      t.text :keywords
      t.string :categoryURL
      t.text :listingIDs

      t.timestamps
    end
  end
end
