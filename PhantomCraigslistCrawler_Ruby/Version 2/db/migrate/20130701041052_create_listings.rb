class CreateListings < ActiveRecord::Migration
  def change
    create_table :listings do |t|
      t.string :userIDs
      t.string :cListNumber
      t.string :title
      t.string :price
      t.string :location
      t.string :date
      t.string :descriptionURL
      t.text :pictureURLs
      t.string :sellersEmail

      t.timestamps
    end
  end
end
