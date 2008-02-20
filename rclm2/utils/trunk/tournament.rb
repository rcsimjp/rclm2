#! /usr/bin/ruby

# library
#

$resultHash = Hash.new
$teamHash = Hash.new


# -----------------------------------------------------------------
# Scheduling

def game(gameID, team1, team2)
#  schedule = File.open( "schedule", "w" )
  unless team1.nil? || team2.nil?          
    unless $resultHash.key?(gameID)        
#      p "Next Match is scheduled"
     puts "#{@homeDir.to_s}#{team1.to_s} vs #{@homeDir.to_s}#{team2.to_s}"
    end
  end
end

# -----------------------------------------------------------------
# Check winner

def winner(gameID) 
  if $resultHash.key?(gameID)
    time, team_l, team_r, score_l, score_r  = $resultHash[gameID]
    if score_l > score_r
      team_l = $teamHash[team_l]
      return team_l
    elsif score_l < score_r
      team_r = $teamHash[team_r]
      return team_r
    end
  end
end

# -----------------------------------------------------------------
# Check Looser

def loser(gameID)
  if $resultHash.key?(gameID)
    time, team_l, team_r, score_l, score_r  = $resultHash[gameID]
    if score_l < score_r
#      team_l = $teamHash[team_l]
      return team_l
    elsif score_l > score_r
#      team_r = $teamHash[team_r]
      return team_r
    end
  end
end

# -----------------------------------------------------------------
# 

def result_store( gameID, column )
  $resultHash[gameID] = column
end

# -----------------------------------------------------------
# 

def load_resultFile(resultFile)           
  matchNum = 1
  io = File.open(resultFile)
  io.each{|line| 
    gameID = "G#{matchNum}"
    line.chomp!
    column = line.split(/\s/)
    result_store( gameID, column )
    matchNum += 1
  }
end

# ------------------------------------------------------------
# 

def set_team( teamName, teamDir )           
    $teamHash[teamName] = teamDir
end

def from_stdin
  matchNum = 1
  while ( line = STDIN.gets )
    gameID = "G#{matchNum}"
    line.chomp!
    column = line.split(/\s/)
    result_store( gameID, column )
    matchNum += 1
  end
end
    


#load_resultFile("result")
#game("G5", "TEST5", "TEST7")

