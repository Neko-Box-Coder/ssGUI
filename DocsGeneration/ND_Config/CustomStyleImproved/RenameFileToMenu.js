let changed = false;
let attached = false;

function func()
{
    // console.log("called");
    var elems = window.document.getElementsByClassName("MTabTitle");

    //If the tab isn't spawned, stop
    if(elems == null)
        return;

    //See if the text is set, if so change it
    for(var i = 0; i < elems.length; i++)
    {
        if(elems[i].innerHTML === "Files")
        {
            elems[i].innerHTML = "📖";
            changed = true;
        }
        else if(elems[i].innerHTML === "📖")
        {
            changed = true;
        }
    }

    //If we have changed the text, we need to hook the title link to trigger the function again since it resets the text
    if(changed)
    {
        // console.log("changed");

        NDMenu.GoToTab("File");
        /*
        var sidebarTab = window.document.getElementById("MTabBar");
        sidebarTab.style.display = "block";
        var fileTab = window.document.getElementById("MFileTab");
        fileTab.className = "MTab Selected Wide";
        var tabChildren = fileTab.children;
        for(var i = 0; i < tabChildren.length; i++)
        {
            if(tabChildren[i].className == "MTabTitle")
            {
                tabChildren[i].innerHTML = "Menu";
                tabChildren[i].style.display = "inline-block";
            }
        }


        var sidebarContent = window.document.getElementById("MContent");
        sidebarContent.style.display = "none";
        */

        //If we have changed the text, we don't need to call this function again
        clearInterval(myInterval);

        //Only hook it once
        if(!attached)
        {
            var title = window.document.getElementById("HTitle");
            var subtitle = window.document.getElementById("HSubtitle");
            const childA = title.querySelector(":scope > a");
            const childA2 = subtitle.querySelector(":scope > a");
            childA.addEventListener("click", function() {changed = false; myInterval = setInterval(func, 100);});
            childA2.addEventListener("click", function() {changed = false; myInterval = setInterval(func, 100);});

            attached = true;
        }
    }
}

// ========================================================================
// Entry Point
// ========================================================================
console.log("root called");

// Keep trying to change the tab from "Files" to "Menu"
myInterval = setInterval(func, 100);
called = true;

