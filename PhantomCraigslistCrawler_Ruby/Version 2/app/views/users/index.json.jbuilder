json.array!(@users) do |user|
  json.extract! user, :firstName, :lastName, :email, :homeURL, :subURL
  json.url user_url(user, format: :json)
end
