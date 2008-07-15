#! /usr/bin/env ruby

if ARGV.empty?
  puts "Usage: #{$0} HH:MM[:SS]"
  exit 0
end

require 'time'

begin
  require 'tk'
  withtk=true
rescue LoadError
  withtk=false
end


$time = Time.parse(ARGV[0])
s = ($time - Time.now).to_i
if s > 0
  puts("Waitting for #{$time}(#{s} s).")
else
  exit
end

if withtk
  # GUI mode.
  $label = TkLabel.new {
    text ' 00:00 '
    font TkFont.new('times 160 bold')
    fg 'red'
    pack('fill' => 'x')
  }
  TkButton.new {
    text 'Skip'
    command { exit 0 }
    pack
  }

  def timer
    n = 0
    TkAfter.new(1000, -1, proc {
                  s = ($time - Time.now).to_i
                  if s > 0
                    m = s / 60
                    ss = s % 60
                    str = sprintf("%02d:%02d", m, ss)
                    # puts "#{s} #{str}"
                    # puts "\a" if s < 10 # ring bell
                    $label.text = " #{str} "
                  else
                    exit
                  end
                }).start
  end

  timer # start timer

  Tk.mainloop
else
  # CUI mode.
  while $time > Time.now
    s = ($time - Time.now).to_i
    if s < 10
      sleep 1
      puts("\a#{s} s.") # \a:bell
    elsif s < 60
      if s == 10
        sleep 1
      elsif 10 < s && s < 20
        sleep (s - 10)
      else
        sleep 10
      end
      puts("The next game will be started after \a#{s} s.") # \a:bell
    elsif s < 120
      sleep (s - 60)
    else
      m = (s / 60).to_i + 1
      sleep 60
    end
  end
end
