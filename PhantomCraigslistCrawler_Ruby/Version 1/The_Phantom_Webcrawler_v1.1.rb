STDOUT.sync = true
require 'open-uri'
require 'pp'
require 'net/http'
require 'date'
require 'tmail'
require 'gmail'
puts "LOADED"
#gmail = Gmail.new("dooodily@gmail.com", "dangermatt505")
#puts "LOGGED ON"

#commands = ['a','2','50','baseball','bat',"-----Original"]
#cmds = commands
#This url will be loaded for watch mode
#We want to know if the price is less than x in this case
#Organize by date, price is after listing
#url = 'http://orlando.craigslist.org/search/sss?sort=date&maxAsk=500&minAsk=2&query=computer&srchType=T'

#This url is for a new keyword search
#We need the first three that are within 2 or 3 weeks
#Organize by price, price is before listing
#url = 'http://orlando.craigslist.org/search/sss?sort=priceasc&max  Ask=500&minAsk=2&query=computer&srchType=T'

def tag_remover(string)
  
  dbug("tag_remover(string)")
    
#puts string
#system("pause")
s=0
n=0
new = ""
is_tag = 0
while s<string.size
  if string[s]=='<'
    is_tag+=1
  end
 # puts 'is_tag = ' + is_tag.to_s + ' s = ' + s.to_s + ' n = ' + n.to_s + ' new = |' + new + '|'
  #system("pause")
  if is_tag == 0
  #  puts '1] is_tag = ' + is_tag.to_s + ' s = ' + s.to_s + ' n = ' + n.to_s + ' new = |' + new + '|'
    new[n] = string[s]
    n+=1
#    puts '2] is_tag = ' + is_tag.to_s + ' s = ' + s.to_s + ' n = ' + n.to_s + ' new = |' + new + '|'
  end
  if string[s]=='>'
    is_tag-=1
  end
  s+=1
  #system("pause")
end
return new
end
#

def load_webpage_return_listings(url)
  
    dbug("load_webpage_return_listings(url)")
    
     lines = Array.new
     list_no = 0
     num = 0
     
     open(url) do |f|
       # This loop pulls up the web page, stores the listings, line by line
       pp  f.meta
       
       f.each do |line|
         print "#{line}"
         
         if list_no>0
           lines[num] = line.to_s
           #puts lines[num]
           num += 1
           #system("pause")
         end
         
         if line.include? '<p class="row">'
           list_no +=1
         end
         if ((line.include? '</p><br><div>sort') || (line.include? '</p><h4'))
           break
         end
         
       end
       
     end
     #
      puts '1st block complete'
     #system("pause")
     
     #Remove as many tags as possible, and separate important information
     i=0
     while i<lines.size
        if lines[i].include? 'class="ih"'
          lines.delete_at(i)
        end
        if lines[i].include? '<a href="'
          lines[i]['<a href="']=' '
        end
        if lines[i].include? '</a>'
          lines[i]['</a>']=' '
        end
        if lines[i].include? '<font size="-1">'
          lines[i]['<font size="-1">']=' '
        end
        if lines[i].include? '</font>'
          lines[i]['</font>']=' '
        end
        if lines[i].include? '$'
          lines[i]['$']=' '
        end
        if lines[i].include? '.html">'
          lines[i]['.html">']='.html '
        end
        if lines[i].include? '</small>'
          lines[i]['</small>']=' '
        end
        if lines[i].include? '<span class="p">'
          lines[i]['<span class="p">']=' '
        end
        if lines[i].include? '</span>'
          lines[i]['</span>']=' '
        end
        if lines[i].include? '<br class="c">'
          lines[i]['<br class="c">']=' '
        end
        while lines[i].include? '-'
          lines[i]['-']=' '
        end
        i+=1
     end
     #
     
      puts '2nd block complete'
     #system("pause")
     
     
     #Here we split the lines by white space into a single dimension array
     a=lines.join(' ')
     a=a.split
     listing = Array.new(list_no) { Array.new}
     
     #puts 'list_no = ' + list_no
     i=0
     k=0
     #Put the listings together with each other
     while i<(list_no)&&a[k-1]!='</p><br><div>sort'&&(a[k-1]!='</p><h4')
        j=0
        while !(a[k] == '<small')&&(a[k]!= '</p><br><div>sort')
          listing[i][j] = a[k]
          #puts a[k] + "1 i = " + i.to_s + " | j = " + j.to_s + "  | k = " + k.to_s
          k+=1
          j+=1
        end
        #Move the pointer past all the bs
        while (a[k] != 'class="row">')&&(a[k]!= '</p><br><div>sort')&&(a[k]!='</p><h4')
          #puts a[k] + "|i = " + i.to_s + " | j = " + j.to_s + "  | k = " + k.to_s
          k+=1
          #system("pause")
        end
        #puts "3 i = " + i.to_s + " | j = " + j.to_s + "  | k = " + k.to_s
        k+=1
        i+=1
     end
     
     
      puts '3rd block complete'
     #system("pause")
     
    return listing
    
end
#

def number_of_weeks_from(month, day)
  
    dbug("number_of_weeks_from(" + month.to_s + ', ' + day.to_s + ')')
   
     #time.strftime("%b %d")
     
     numth = 0
     
     if month == 'Jan'
          numth = 1
     end
     if month == 'Feb'
          numth = 2
     end
     if month == 'Mar'
          numth = 3
     end
     if month == 'Apr'
          numth = 4
     end
     if month == 'May'
          numth = 5
     end
     if month == 'Jun'
          numth = 6
     end
     if month == 'Jul'
          numth = 7
     end
     if month == 'Aug'
          numth = 8
     end
     if month == 'Sep'
          numth = 9
     end
     if month == 'Oct'
          numth = 10
     end
     if month == 'Nov'
          numth = 11
     end
     if month == 'Dec'
          numth = 12
     end
     
     if numth == 0
       return 52
     end
     if day == 0
       return 52
     end
     
     date = Date.new(2011, numth, day)
     #puts date
     today = DateTime.now
     days_elapsed = today - date
     if (days_elapsed <= 7)
          weeks = 1
     elsif (days_elapsed <= 14)
          weeks = 2
     elsif (days_elapsed <= 21)
          weeks = 3
     else
          weeks = 4
     end
     
     return weeks
     
end
#

def add_new_keywords(cmds, k)
  
    dbug("add_new_keywords(cmds, k), k = |" + k.to_s + "|")
    
   #This block sets up the two URL's: request is for the initial search,
   #And, the key url will be saved into a file to be pulled up later
   #k=1#Supposed to be k+=1??
   request = 'http://orlando.craigslist.org/search/sss?srchType=T&minAsk='
   if cmds[k].to_i!=0##Min price
     request+=cmds[k]
     k+=1
   else
     request+='2'
   end
   if cmds[k].to_i!=0##Max price
     request+='&maxAsk='
     request+=cmds[k]
     k+=1
   end
   key = request
   request+='&sort=priceasc&query='
   key+='&query='
   while ((cmds[k]!='*')&&(cmds[k]!="-----Original"))##Keywords
     request+=cmds[k]
     request+="+"
     key+=cmds[k]
     key+="+"
     k+=1
   end
   k-=1       
   
   
   
   puts 'after 1st block'
   #system("pause")
   
   #Add new keyword URL to keywords.txt
   if (File.exists?("keywords.txt")) 
     file = File.open("keywords.txt", "a+")
   else
     file = File.new("keywords.txt", "w+")
   end
   file.puts key
   file.close   
   
   dbug(request)
   
   #strip the listings
   listings = load_webpage_return_listings(request)
   
   dbug(listings.join('-'))
   
   q = number_check_and_clean_listings(listings)
   
   dbug('q=' + q)
   
   return q
   
end
#

def number_check_and_clean_listings(listings)
  
   dbug("number_check_and_clean_listings(listings)")
   
   to_send = Array.new
   num_weeks = 1
   list_num = 0
   
   #This messy loop checks the price ordered list for items listed w/in 1 weeks, then 2...
   while ((to_send.size < 3)&&(num_weeks<5)) #This tries to get 3 listings
      while ((list_num < listings.size)&&(to_send.size < 3)) #This checks within the time period
        
        #Check if it's within however many weeks
        if (number_of_weeks_from(listings[list_num][0],listings[list_num][1].to_i)<=num_weeks)
          
          #Check if it has been sent yet
          if (has_this_been_sent(listings[list_num][2])==(-1))
            
            #Create or add to file
            if (File.exists?("items.txt")) 
                file = File.open("items.txt", "a+")
            else
                file = File.new("items.txt", "w+")
            end
            file.puts listings[list_num][2]
            
            #Now count the items in the item file
            file.rewind
            count=0
             begin
                while (line = file.readline)
                  count+=1
                end
                rescue EOFError
                  file.close
             end
            
            #put everything that we want to send in the to_send variable
            to_send[to_send.size] = '|' + count.to_s + '~' + listings[list_num][0] + listings[list_num][1] + '$' + listings[list_num][3]
            wrd_num = 4
            while wrd_num < listings[list_num].size
                 to_send[to_send.size - 1] += ' ' + listings[list_num][wrd_num]
                 wrd_num+=1
            end
         
          end
          #
         
        end
        #
        list_num+=1
        
      end
     list_num = 0
     num_weeks+=1 
   end
   #
    
    dbug(to_send.join)
    
    return to_send.join
    
end

#Check if the item url is already in items.txt, return -1 if not

def has_this_been_sent(string)

  dbug("has_this_been_sent(string)")
  
     #Open items.txt, make sure you're in the right directory
     #Create or add to file
     if (File.exists?("items.txt")) 
        f = File.open("items.txt", "r+")
     else
        return (-1)
     end
     
     #Put the lines into an array
     x = 0
     lines = Array.new
     begin
          while (line = f.readline)
               lines[x] = line.chomp
               #$stdout.print line
               x+=1
          end
     rescue EOFError
          f.close
     end
     #
     
     #Check for the strings
     y=0
     while y<x
          if lines[y]==string
               break
          end
          y+=1
     end
     #
  # end  
     #Return -1 if no match has been found, otherwise return the element number
     if y==x
          return (-1)
     else
          return y
     end
#

end
#

def load_post(url)
     
  dbug("load_post(url), url = |" + url.to_s + "|")
  
   post = Array.new(2) { Array.new}
   a = ""
   start_scanning = 0
   open(url) do |f|
     # This loop pulls up the web page, stores the listings, line by line
     pp  f.meta
     
     f.each do |line|
       print "#{line}"
       
       if start_scanning == 1
         a+= tag_remover(line.to_s)
       end
       
       if line.include? 'Reply to:'
         if line.include? 'Reply to: see below'
         post[0] = nil
         else
         post[0] = tag_remover(line.to_s)
         post[0]["Reply to: "]= ""
         post[0]['[Errors when replying to ads?]']= ""
         post[0] = post[0].split.join
         end
         
         start_scanning = 1
       end
       
       
       if (line.include? 'class="blurbs">')
         break
       end
       
     end
      
   end

   post[1] = a.split.join(' ')
   
   return post
end
#

def find_item(itnum)
  
  itnum = itnum.to_i
  
   dbug("find_item(itnum)), itnum = |" + itnum.to_s + "|")
  
  #Add new keyword URL to keywords.txt
  if (File.exists?("items.txt")) 
    file = File.open("items.txt", "r+")
  else
    return nil
  end
  
  num = 0  
  begin
    while num < itnum
      line = file.readline.chomp#see what type of variable line is
      num+=1
    end
  rescue EOFError
    file.close
    return nil
  end
  
  file.close
  #system("pause")
  c = load_post(line)
  return c
  
end
#

def compare_listings(new,old)
  
 dbug("compare_listings(new,old)")
   
   difference = Array.new(2) { Array.new}
   
   i=0
   k=0 # We will make k's max to be 3; or a maximum of three returned listings in an array
   while((i<new.size) && (k<3))
      
      j=0
      # This loop will be broken out of if the new listing was found in the old list
      while(j<old.size)
        if(new[i][2]==old[j][2])
          break
        end
        j+=1
      end
      
      # If j made it through the loop, without breaking, the new listing was not found
      # Add it to the outgoing list
      if(j==old.size)
        #puts new[i].to_s
        difference[k] = new[i]
        k+=1
      else
        # If it hits once, forget it; there are no new listings
        break
      end
        
      i+=1
   end
   
   # This if will activate if nothing at all was found
   if difference[0] == []
     return nil
   else
     return difference
   end
   
end
#
def write_array_to_file(array, filename)
  
   dbug("write_array_to_file(array, filename)")
 
  fp = File.new(filename, "w")# Overwrite
  
  i = 0  
  
    while i < array.size
      
      j=0
      while j < array[i].size
        
        fp.puts array[i][j].to_s
        
        j+=1
      end
      
      fp.puts '~'
      
      i+=1
    end

  
  fp.close
  
end
#

def read_array_from_file(filename)
  
  dbug("read_array_from_file(filename)")
  
  # Open the file, if it exists
  if(File.exists?(filename))
    fp = File.open(filename, "r+")
  else
    return nil
  end
  
  # Count how many array spots there will be
  count = 0  
  begin
    while (1==1) 
      line = fp.readline.chomp
      if line == '~'
        count+=1
      end 
    end  
  rescue EOFError
    fp.rewind
  end

  array = Array.new(count) { Array.new}
  
  i = 0  
  begin
    while i < count
      j=0
      while (1==1)
        line = fp.readline.chomp
        if line != '~'
          array[i][j] = line.to_s
          j+=1
        else
          break
        end
      end 
      i+=1
    end
  rescue EOFError
    fp.close
    return array
  end
  return array
end
#

def dbug(strng)
 
  curdir=Dir.pwd
  Dir.chdir('C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler')
  file = File.new("DEBUG.txt", "a")
  
  puts DateTime.now.hour.to_s + ':' + DateTime.now.min.to_s + ':' + DateTime.now.sec.to_s + '> ' + strng + "\n"
  file.puts DateTime.now.hour.to_s + ':' + DateTime.now.min.to_s + ':' + DateTime.now.sec.to_s + '> ' + strng + "\n"
  file.close
  
  crashfile = File.new("Crash_watcher.txt", "w")
  crashfile.puts DateTime.now.hour.to_s + ':' + DateTime.now.min.to_s + ':' + DateTime.now.sec.to_s
  crashfile.close
  
  Dir.chdir(curdir)
  return curdir
  
end
#

max_keywords = 10
maxusrs = 10
i = 0

master_while = 0


#system("pause")
while (true)
  
  
  dbug("BEGIN")
   
  i = 0
   while (i<maxusrs)
    
    dbug("TOP>> MW: " + master_while.to_s + ', Usr ' + i.to_s)
    
    gmail = Gmail.new("dooodily@gmail.com", "dangermatt505")
    
    dbug("LOGDON")
   
    #Get into the user's directory, create if it doesn't exist
    direct = 'C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s
    if !(Dir.exist?(direct))
      Dir.mkdir(direct)
    end
    Dir.chdir(direct)
    
    #Fetch the usr's texting address
    file = File.new("cell.txt", "r+") 
    usr = file.gets
    file.close
    
    
    #Email status printer
    puts "gmail.inbox.count = " + gmail.inbox.count.to_s
    puts "gmail.inbox.count(:unread) = " + gmail.inbox.count(:unread).to_s
    puts ':from => "mbeck@knights.ucf.edu" :  ' +  gmail.inbox.count(:from => "mbeck@knights.ucf.edu").to_s + ', and ' + gmail.inbox.count(:unread, :from => "mbeck@knights.ucf.edu").to_s + ' are unread'
    puts ':from => "8634140273@txt.att.net" : ' +  gmail.inbox.count(:from => "8634140273@txt.att.net").to_s + ', and ' + gmail.inbox.count(:unread, :from => "8634140273@txt.att.net").to_s + ' are unread'
    puts "user number " + i.to_s + ', |' + usr + '|, has ' + gmail.inbox.count(:from => usr).to_s + ' emails, ' + gmail.inbox.count(:unread, :from => usr).to_s + ' of which are unread'
    sleep 6
    
    ##########E-mail Retriever##########################
    while (gmail.inbox.count(:unread, :from => usr)>0) 
      
      ########Get the usr's new email(s)################
      email = gmail.inbox.emails(:unread, :from => usr).first
      if email!=nil
         commands = email.body.to_s.split
      else
         commands = nil
      end
      
      ###############
      #The cell command loop
      ###############
      k=0
      while (commands!=nil && commands[k]!="-----Original")#This worked in interactive Ruby, but could crash the program
        if (commands[k]=='*')
          k+=1
          request = ""
          
          if (commands[k]=='E'||commands[k]=='e')#View Entire Post 
            k+=1
            item_number = commands[k]#Should be the Item Number
            item = find_item(item_number)
            
            if item!=nil
              bod = item[1]
            else
              bod = 'There was an error processing your request'
            end
            
            #Eventually make this so it only sends 3 texts at a time, and the user
            #must ask for moar to get the next 3
            gmail.deliver do
                   to usr
                   subject i.to_s
                   text_part do
                     body bod
                   end
            end
            
          end 
          if (commands[k]=='I'||commands[k]=='i')#View Images
            k+=1
            itemnum = commands[k]
            
            
          end
          if (commands[k]=='B'||commands[k]=='b')#Default Response to Buy
            k+=1
            item_number = commands[k]#Should be the Item Number
            item = find_item(item_number)
            
            if item[0]!=nil
              recipient = item[0]
              bod "Hello,\nI am interested in what you posted on Craigslist; send an email to my phone:\n\n" + usr + "\n\nor just call the number right before the @ symbol.\n\nThanks!!!"
            else
              recipient = usr
              bod = 'There was an error processing your request'
            end
            
            gmail.deliver do
                   to recipient
                   subject "Craigslist"
                   text_part do
                     body bod
                   end
            end
            
            
          end 
          if (commands[k]=='R'||commands[k]=='r')#Custom Response
            k+=1
            item_number = commands[k]#Should be the Item Number
            item = find_item(item_number)
            
            response = ""
            while ((commands[k]!='*')&&(commands[k]!="-----Original"))
              response+=commands[k]
              response+=" "
              k+=1
            end
            
            if item[0]!=nil
              recipient = item[0]
              bod = response
            else
              recipient = usr
              bod = 'There was an error processing your request'
            end
            
            gmail.deliver do
                   to recipient
                   subject "Craigslist"
                   text_part do
                     body bod
                   end
            end
            
            k-=1
          end 
          if (commands[k]=='A'||commands[k]=='a')#Add Keywords
               k+=1
               
               bod = add_new_keywords(commands,k)
               
               dbug("bod= " + bod)
               
               puts 'out of add_new_keywords(commands,k)'
               #system("pause")
               
               #Send the three listings just found
               gmail.deliver do
                   to usr
                   subject i.to_s
                   text_part do
                     body bod
                   end
               end
              
          end   
          if (commands[k]=='C'||commands[k]=='c')#Clear Keyword List
            if (File.exists?("keywords.txt")) 
              File.delete("keywords.txt")
            end
          end 
          if (commands[k]=='L'||commands[k]=='l')#View Keyword List
          
          end         
          
        end
        k+=1
      end
      # 
    end
    
    dbug("COMMANDS EXECUTED")
    sleep 3
    
    #Check for updated listings
    if (File.exists?("keywords.txt")) 
      
      keyfile = File.open("keywords.txt", "r+")
     
      #Get into the search directory; create if it doesn't exist
      direct = 'C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s + '\searches'
      if !(Dir.exist?(direct))
        Dir.mkdir(direct)
      end
      Dir.chdir(direct)
      #BEWARE: This is complicating because keywords.txt is still open, but we're now in a different directory
     
     num = 0  
     begin
       while (num < max_keywords)
         
          dbug(num.to_s + ":\n")
         
         ### Get the new listings from a webpage, and the old ones from a file
         line = keyfile.readline.chomp
         newlistings = load_webpage_return_listings(line)
         flnm = "key" + num.to_s + ".txt"
         oldlistings = read_array_from_file(flnm)
        
         # Only generate fresh if there are old listings to compare the new ones with
         if oldlistings != nil
           fresh = compare_listings(newlistings,oldlistings)
         else
           # If there are no old listings, put them in there
           write_array_to_file(newlistings, flnm)
           fresh = nil
         end
         
         # If fresh has something, that means the webpage changed
         if fresh != nil
           
           dbug("Keyword webpage changed, sending email")
           
           puts 'going into number_check_and_clean_listings(listings)'
           #system("pause")
           direct = 'C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s
           Dir.chdir(direct)
           bod = number_check_and_clean_listings(fresh)
           direct = 'C:\\Documents and Settings\Matt\Desktop\The Phantom Webcrawler\user' + i.to_s + '\searches'
           Dir.chdir(direct)
           puts "just exited number_check_and_clean_listings(listings), it returned:\n\n" + bod.to_s
           #system("pause")
           
           gmail.deliver do 
                   to usr
                   subject i.to_s
                   text_part do
                     body bod
                   end
           end
           # And update the oldlistings file
           write_array_to_file(newlistings, flnm)
         end
         
         #puts 'block 2 done, num = ' + num.to_s
         #system("pause")
         
         num+=1 # Go to the next key word
       end
     rescue EOFError
       
     end
     
    end
    
    dbug("KEYSEARCH DONE")
    
    gmail.logout
    gmail = nil
    
    dbug("LOGDOUT")
    sleep 2
    i+=1#Move on to the next user
    
   end
   
   puts "MADE IT!!!!\n\n"
   sleep 4
   master_while += 1 
   
end