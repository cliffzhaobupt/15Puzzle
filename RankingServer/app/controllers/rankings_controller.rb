class RankingsController < ApplicationController
  skip_before_filter :verify_authenticity_token

  def get
    # sleep 2
    ranking_items = Ranking.select(:id, :time, :name).order("time")
    render json: ranking_items
  end

  def add
    new_ranking_item = Ranking.new(
      name: params[:name],
      time: params[:time]
      )
    if new_ranking_item.save
      render json: {success: true}
    else
      render json: {success: false}
    end
  end
end
