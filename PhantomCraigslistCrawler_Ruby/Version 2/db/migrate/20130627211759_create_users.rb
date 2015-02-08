class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :firstName
      t.string :lastName
      t.string :email
      t.string :homeURL
      t.string :subURL

      t.timestamps
    end
  end
end
