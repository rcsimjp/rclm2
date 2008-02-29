#! /usr/bin/ruby

class TeamNameLoader
  def initialize
    @teams = []
  end

  def from_stdin
    while ( team = STDIN.gets )
      team = team.chomp.tr_s(" \t", '_')
      if ( team.empty? )
        next
      end
      @teams << team
      #   }
    end
    @teams = @teams.uniq
  end

  def print
    @teams.each { | team |
      puts team
    }
  end

  def getTeams
    return @teams
  end
end

class Scheduler
  def initialize( total_teams )
    create( total_teams )
  end

  def create( total_teams )
    @total_teams = total_teams
    total_matches = total_teams * (total_teams - 1) / 2
    @match_list = []

#    p total_teams
    1.upto(total_matches) { | match |
#      p match
      k = [1, total_teams]
      i = 0;
      down = 0;

      m = 1;
      while ( m < match )
        if ( i + k[down] < total_teams - 1 )
          i += 1
        else
          i = 0;
          if ( down == 0 )
            down = 1
            k[down] -= 1
          else
            down = 0
            k[down] += 1
          end
        end
        m += 1
      end

      left = i
      right = k[down] + i;

      match = [ left, right ]
      if ( rand() % 2 )
        match = [ right, left ]
      end
      @match_list << match
    }
  end

  def getMatchList
    return @match_list;
  end

  def print
    @match_list.each { | m |
      puts "#{m[0]} vs #{m[1]}"
    }
  end
end

def usage
  STDERR.puts "Usage: rclmscheduler < [TeamListedFile]"
end


## Main
loader = TeamNameLoader.new

loader.from_stdin

if ( loader.getTeams.empty? )
  usage
  puts "Team list is empty!"
  exit 1
end

scheduler = Scheduler.new( loader.getTeams.size )

teams = loader.getTeams
matches = scheduler.getMatchList
STDERR.puts "The number of teams : " + teams.size.to_s
STDERR.puts "The number of matches : " + matches.size.to_s

matches.each { | m |
  puts "#{teams[m[0]]} vs #{teams[m[1]]}"
}
