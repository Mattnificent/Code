json.array!(@searches) do |search|
  json.extract! search, :userIDs, :minPrice, :maxPrice, :keywords, :categoryURL, :listingIDs
  json.url search_url(search, format: :json)
end
