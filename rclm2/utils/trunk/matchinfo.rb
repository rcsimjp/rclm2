#! /usr/bin/ruby

require 'tempfile'

# ------------------------------------------------
# �Ʒ�̤���¸���뤿�����������

class ArrangeResult
  attr_reader :resultHash, :teamHash, :infoline
  attr_accessor :teamname, :gameID

  def initialize(lines)
    @lines = lines
    @resultHash = Hash.new   # ������ǡ����� ((name ������̾) ���� ���� ��� ��ʬ ���� ����)
    @teamHash = Hash.new {|hash,key|
      hash[key] = [0,0,0,0,0,0]
    }
  end

  # -----------------------------------------------------------
  # result�ե�������ɤ߹���
  
  def load_resultFile
    matchNum = 1
    @lines.each{|line|
      gameID = "G#{matchNum}"
      line.chomp!
      column = line.split(/\s/)
      result_store( gameID, column )
      matchNum += 1
    }
    return @resultHash
  end
  
  # -----------------------------------------------------------------
  # ��̤��������¸
 
  def result_store( gameID, column )
    @resultHash[gameID] = column
  end
  
  # -----------------------------------------------------------------
  # ����̤��ǧ���ơ�������ǡ����򹹿�
  
  def checkMatchResult(gameID) 
    if @resultHash.key?(gameID)
      time, team_l, team_r, score_l, score_r, pk_l, pk_r  = @resultHash[gameID]
      @teamHash[team_l][0] += 1
      @teamHash[team_r][0] += 1
      @teamHash[team_l][4] += score_l.to_i
      @teamHash[team_l][5] += score_r.to_i
      @teamHash[team_r][4] += score_r.to_i
      @teamHash[team_r][5] += score_l.to_i
      if score_l > score_r
        @teamHash[team_l][1] += 1
        @teamHash[team_r][2] += 1
      elsif score_l < score_r
        @teamHash[team_r][1] += 1
        @teamHash[team_l][2] += 1
      elsif score_l == score_r
        @teamHash[team_l][3] += 1
        @teamHash[team_r][3] += 1

        unless pk_l == nil
#          puts "pk mode"
          if pk_l > pk_r
            puts "#{pk_l}"
            puts "#{pk_r}"
#           puts "pk left win"
            @teamHash[team_l][1] += 1
            @teamHash[team_r][2] += 1
          elsif pk_l < pk_r
#           puts "pk right win"
            @teamHash[team_r][1] += 1
            @teamHash[team_l][2] += 1
          end
          @teamHash[team_l][3] = @teamHash[team_l][3] - 1
          @teamHash[team_r][3] = @teamHash[team_r][3] - 1
        end

      end
    end
  end
  
  # ---------------------------------------------------------------
  # ������ǡ����Ȼ��ǡ�������η����ˤ��ƽ��Ϥ���
  #((name ������̾) ���� ���� ��� ��ʬ ���� ����)
  #((team ������1̾) (team ������2̾) ������1 ������2 )  
  def teamDataOutput

    @resultline = ""
    @teamline = ""
    
    infoline = ""
    
    @resultHash.each_key{|key|
      checkMatchResult(key)
    }
    return @teamHash
  end
end


class CreateDataFile
  attr_reader :buffersize, :results, :matchInfoFile, :infomessage
  def initialize (results)
    @results = results
    @matchInfoFile = File.open("matchinfo","w")
    @matchData = @results.load_resultFile
    @teaminfo = @results.teamDataOutput
  end
  
  def createFile
    @matchInfoFile.puts("(results ")
  end

  def outputTeamData
    @matchInfoFile.puts(" (teams ")
    @teaminfo.each_key{|key|
      @matchInfoFile.puts "  ((name #{key}) #{@teaminfo[key][0]} #{@teaminfo[key][1]} #{@teaminfo[key][2]} #{@teaminfo[key][3]} #{@teaminfo[key][4]} #{@teaminfo[key][5]})"
    }
    @matchInfoFile.puts(" )")
  end
  
  def outputMatchResult
    @matchInfoFile.puts(" (matches" )    
    @matchData.each_key{|gameID|
      unless( @matchData[gameID][5] == nil )
        @matchInfoFile.puts("  ((team #{@matchData[gameID][1]}) (team #{@matchData[gameID][2]}) #{@matchData[gameID][3]} #{@matchData[gameID][4]} #{@matchData[gameID][5]} #{@matchData[gameID][6]})")
      else
        @matchInfoFile.puts("  ((team #{@matchData[gameID][1]}) (team #{@matchData[gameID][2]}) #{@matchData[gameID][3]} #{@matchData[gameID][4]})")
      end
    }
    @matchInfoFile.puts(" )" )
  end
  
  def endFile
    @matchInfoFile.puts(")" )
    @matchInfoFile.close
  end
end

def readlines_from_files(files)
  if files.empty?
    lines = STDIN.readlines
  else
    lines = []
    files.each { | f |
      File.open(f, 'r') { | fin |
        l = fin.readlines
        lines.concat l
      }
    }
  end
  lines
end

def usage
  STDERR.print "usage: [-t|-m] \n"
end

## Main
while ARGV[0] =~ /^-/
  $_ = ARGV.shift  
  files = ARGV
  lines = readlines_from_files(files)
  
  results = ArrangeResult.new(lines)
  datafile = CreateDataFile.new(results)
  
  if($_ =~ /^-t/)
    p "teammode"
    datafile.createFile
    datafile.outputTeamData
    datafile.endFile
    exit
  elsif($_ =~ /^-m/)
    p "resultmode"
    datafile.createFile
    datafile.outputMatchResult
    datafile.endFile
    exit
  else
    p "usage" 
    usage
    exit 1
  end
end

p "nomalMode"
files = ARGV
lines = readlines_from_files(files)

results = ArrangeResult.new(lines)
datafile = CreateDataFile.new(results)

datafile.createFile
datafile.outputTeamData
datafile.outputMatchResult
datafile.endFile
