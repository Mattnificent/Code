json.array!(@listings) do |listing|
  json.extract! listing, :userIDs, :cListNumber, :title, :price, :location, :date, :descriptionURL, :pictureURLs, :sellersEmail
  json.url listing_url(listing, format: :json)
end
