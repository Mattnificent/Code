STDOUT.sync = true
#require 'net/smtp'
#require 'net/imap'
require 'net/http'
require 'tmail'
require 'gmail'
puts "LOADED"
gmail = Gmail.new("dooodily@gmail.com", "dangermatt505")
puts "LOGGED ON"

maxusrs = 5
#system("pause")
while (true)
  
  i = 0
  while (i<maxusrs)
    
    #Get into the user's directory, create if it doesn't exist
    if !(Dir.exist?('C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s))
      Dir.mkdir('C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s)
    end #make the directory a variable because this next line is being a bitch
    Dir.chdir('C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + 1.to_s)
    
    file = File.new("cell.txt", "r+") 
    usr = file.gets
    file.close
    #puts "user number " + i.to_s + ' is |' + usr + '|'
    #system("pause")
    
    #Check if anything needs to be sent
    if (File.exists?("send.txt"))
     
      file = File.open("send.txt", "r+") 
      bod = file.gets
      file.close
      #usr = "mbeck@knights.ucf.edu"
      #usr = "8634140273@txt.att.net"
      ##########E-mail Sender##########################
      gmail.deliver do
        to usr
        subject i.to_s
        text_part do
          body bod
        end
      end
      File.delete("send.txt")
    end
    #usr = "mbeck@knights.ucf.edu"
    #usr = "8634140273@txt.att.net"
    ##########E-mail Retriever##########################
    while (gmail.inbox.count(:unread, :from => usr)>0) 
      #puts gmail.inbox.count(:unread, :from => usr).to_s + "from the user are unread"
      #system("pause")    
      #puts email + "\n~\n\n\nThis is the user's " + j.to_s + "th new message"
      #system("pause")
      
      ##### '*' + ##### Needed for text email
      email = gmail.inbox.emails(:unread, :from => usr).first.body.to_s
      file = File.new("wtfisthis.txt","w+")
      
      puts email
      file.puts email
      
      
      commands = email.split
      
      
      g=0
      while (commands[g]!="-----Original")
      puts g.to_s + commands[g]
      file.puts g.to_s + commands[g]
      g+=1
      end
      file.close
      
      k=0
      while (commands[k]!="-----Original")
        if (commands[k]=='*')
          k+=1
          request = ""
          filename = "dummy.txt"
          
          if (commands[k]=='E'||commands[k]=='e')   #View Entire Post
            filename = "postreq.txt"
            k+=1
            
            request = commands[k]                   #Should be the Item Number
           
          elsif (commands[k]=='I'||commands[k]=='i')#View Images
            filename = "imgreq.txt"
            k+=1
            
            request = commands[k]
           
          elsif (commands[k]=='B'||commands[k]=='b')#Default Response to Buy
            filename = "defresp.txt"
            k+=1
            
            request = commands[k]
           
          elsif (commands[k]=='R'||commands[k]=='r')#Custom Response
            filename = "custresp.txt"
            k+=1
            
            while ((commands[k]!='*')&&(commands[k]!="-----Original"))
              request+=commands[k]
              request+=" "
              k+=1
            end
           
            k-=1
           
          elsif (commands[k]=='A'||commands[k]=='a')#Add Keywords
            filename = "newkeywds.txt"
            k+=1
            
            request = 'http://orlando.craigslist.org/search/sss?srchType=T&minAsk='
            request+=commands[k]
            request+='&maxAsk=&sort=priceasc&query='
            k+=1
            while ((commands[k]!='*')&&(commands[k]!="-----Original"))
              request+=commands[k]
              request+="+"
              k+=1
            end
            
            k-=1
            
          elsif (commands[k]=='C'||commands[k]=='c')#Clear Keyword List
            
           
           
          elsif (commands[k]=='L'||commands[k]=='l')#View Keyword List
            
            
            
          end
         
          if (File.exists?(filename)) #Create or add to file
            file = File.open(filename, "a")
          else
            file = File.new(filename, "w")
          end
          
          file.puts request
          file.puts "~"
          file.close
         
        end
        k+=1
      end
      
    end
    i+=1
  end

end