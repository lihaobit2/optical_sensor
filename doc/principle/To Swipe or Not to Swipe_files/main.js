function myForm1Check(description)
{ 
    //Check required fields for user input
    if (!myCheckRadioDemos(document.qualform.RECEIVE_YES_NO, 'pReceiveYesNoError', 'SUBSCRIPTION_INFO_FIELDSET'))
    {   
        document.qualform.RECEIVE_YES_NO[0].focus();
        return false;
    }
    else if (!myCheckRadioDemos(document.qualform.REQUESTED_VERSION, 'pRequestedVersionError', 'SUBSCRIPTION_INFO_FIELDSET'))
    {   
        document.qualform.REQUESTED_VERSION[0].focus();
        return false;
    }          
    else if (!myCheckFormElement('txtFirstName', 'liFirstNameError', 'First Name'))
    {   
        return false;
    }
    else if (!myCheckFormElement('txtLastName', 'liLastNameError', 'Last Name'))
    {   
        return false;
    }
    else if (!myCheckFormElement('txtJobTitle', 'liJobTitleError', 'Job Title'))
    {   
        return false;
    }         
    else if (!myCheckFormElement('txtCompanyName', 'liCompanyNameError', 'Company Name'))
    {   
        return false;
    }
    else if (!myCheckFormElement('txtStreetAddress', 'liStreetAddressError', 'Company Street Address'))
    {   
        return false;
    }      
    //else if (!myCheckFormElement('txtDepartment', 'liDepartmentError', 'Department/Mail Stop'))
    //{   
    //    return false;
    //}   
    else if (!myCheckFormElement('txtCity', 'liCityError', 'City'))
    {   
        return false;
    }    
    else if (!myCheckFormElement('state', 'liStateError', ''))
    {   
        return false;
    }                  
    else if (!myCheckFormElement('Country', 'liCountryError', ''))
    {   
        return false;
    } 
    else if (!myCheckValidCountry())
    {
        return false;
    }    
    else if (!myCheckValidState())
    {
        return false;
    }    
    else if (!myCheckFormElementLength10zip('txtZIP', 'liZipError', 'ZIP/Postal Code'))
    {   
        return false;
    }     
    else if (!myCheckFormElementISNumeric('txtPhone', 'liPhoneError', 'Business Phone'))
    {   
        return false;
    } 
	else if (!myCheckFormElementLength10phone('txtPhone', 'liPhoneError', 'Business Phone'))
    {   
        return false;
    }  
    else if (!myCheckFormElement('txtEmailAddress', 'liEmailAddressError', 'Email'))
    {   
        return false;
    }    
    else if (!myCheckValidEmail('txtEmailAddress', 'liInvalidEmailAddressError'))
    {   
        return false;
    }
    else if (!myCheckFormElement('txtConfirmEmail', 'liConfirmEmailError', 'Confirm Your Email Address'))
    {   
        return false;
    }    
    else if (!myCheckConfirmEmail('txtEmailAddress', 'txtConfirmEmail', 'liInvalidConfirmEmailError'))
    {   
        return false;
    }
    else if (!myCheckSpecialLogic())
    {
        return false
    }
    else
    {
        return true;
    }
} 

function myCheckSpecialLogic()
{
    //If user chooses not to start or renew the subscription and hit continue, they go back to the landing page
    if(document.qualform.RECEIVE_YES_NO[1].checked == true)
    {
        if (confirm('You have selected not to start or renew a FREE subscription to CED magazine. Continue?'))
        {
            parent.location='http://subscribe.advantagemedia.com/ced/landing.aspx';
            return false;
        }
        else
        {
            return false;
        }
    }

    //If user selects digital, they go on to second form
    if(document.qualform.REQUESTED_VERSION[0].checked == true)
    {
        return true;
    }
    else
    {
        //If user selects print and they are from USA or Canada, they go on to second form
        if(document.getElementById('Country').value == "United States" || document.getElementById('Country').value == "Canada")
        {
            return true;      
        }
        else
        {
            //If user selects print and they are from outside USA or Canada, and they answer yes to paid subscription, they go on to paid form
            if (confirm('A FREE print subscription to CED magazine is for USA and Canada Only. Do you wish to subscribe for a PAID print subscription?'))
            {
                parent.location='https://advantage.omeda.com/cgi-win/abced.cgi?paid';
            }
            //else they go back to first form
            else
            {
                return false;
            }
        }
    }
}

function isNumeric(input) {
    //var number = /^\-{0,1}(?:[0-9]+){0,1}(?:\.[0-9]+){0,1}$/i;
    var number = /^(?:[0-9]+)$/i;
var regex = RegExp(number);
    return ((regex.test(input)) || (input=='Business Phone')) ;
}

function trim(stringToTrim) {
                return stringToTrim.replace(/^\s+|\s+$/g,"");
}
function ltrim(stringToTrim) {
                return stringToTrim.replace(/^\s+/,"");
}
function rtrim(stringToTrim) {
                return stringToTrim.replace(/\s+$/,"");
}

function isNumeric(input) {
    //var number = /^\-{0,1}(?:[0-9]+){0,1}(?:\.[0-9]+){0,1}$/i;
    var number = /^(?:[0-9]+)$/i;
var regex = RegExp(number);
    return ((regex.test(input)) || (input=='Business Phone')) ;
}

function trim(stringToTrim) {
                return stringToTrim.replace(/^\s+|\s+$/g,"");
}
function ltrim(stringToTrim) {
                return stringToTrim.replace(/^\s+/,"");
}
function rtrim(stringToTrim) {
                return stringToTrim.replace(/\s+$/,"");
}



function myCheckFormElementISNumeric(fieldName, errorFieldName, defaultValue)
{ 
var val=document.getElementById(fieldName).value
                                if(val == "Business Phone")
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                                return true;
                                }
                                if(!isNumeric(val))
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(fieldName).focus();
            document.getElementById(fieldName).style.backgroundColor="#f9ee8f";
            document.getElementById(fieldName).style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckFormElementLength10zip(fieldName, errorFieldName, defaultValue)
{ 
var val=document.getElementById(fieldName).value
                                if(val.length > 10)
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(fieldName).focus();
            document.getElementById(fieldName).style.backgroundColor="#f9ee8f";
            document.getElementById(fieldName).style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckFormElementLength10phone(fieldName, errorFieldName, defaultValue)
{ 
var val=document.getElementById(fieldName).value
                                if(val == "Business Phone")
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                                return true;
                                }
                                if(val.length > 10)
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(fieldName).focus();
            document.getElementById(fieldName).style.backgroundColor="#f9ee8f";
            document.getElementById(fieldName).style.borderWidth="2px";
                                                return false;
                                }
				else					
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckFormElement(fieldName, errorFieldName, defaultValue)
{
                                if(document.getElementById(fieldName).value == defaultValue)
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(fieldName).focus();
            document.getElementById(fieldName).style.backgroundColor="#f9ee8f";
            document.getElementById(fieldName).style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckValidState()
{
                                if(document.getElementById('state').value != "NA" && (document.getElementById('Country').value != "United States" && document.getElementById('Country').value != "Canada"))
                                {
            document.getElementById('liStateCountryMismatchError').style.display="block";
                document.getElementById('state').focus();
            document.getElementById('state').style.backgroundColor="#f9ee8f";
            document.getElementById('state').style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById('liStateCountryMismatchError').style.display="none";
            document.getElementById('state').style.backgroundColor="#ffffff";
            document.getElementById('state').style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckValidCountry()
{
                                if(document.getElementById('state').value == "NA" && (document.getElementById('Country').value == "United States" || document.getElementById('Country').value == "Canada"))
                                {
            document.getElementById('liCountryStateMismatchError').style.display="block";
                document.getElementById('Country').focus();
            document.getElementById('Country').style.backgroundColor="#f9ee8f";
            document.getElementById('Country').style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById('liCountryStateMismatchError').style.display="none";
            document.getElementById('Country').style.backgroundColor="#ffffff";
            document.getElementById('Country').style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckValidEmail(fieldName, errorFieldName)
{
                                if(!validEmail(document.getElementById(fieldName).value))
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(fieldName).focus();
            document.getElementById(fieldName).style.backgroundColor="#f9ee8f";
            document.getElementById(fieldName).style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(fieldName).style.backgroundColor="#ffffff";
            document.getElementById(fieldName).style.borderWidth="1px";
                                    return true;
                                }
}

function myCheckConfirmEmail(field1Name, field2Name, errorFieldName)
{
                                if(document.getElementById(field1Name).value.toUpperCase() != document.getElementById(field2Name).value.toUpperCase())
                                {
            document.getElementById(errorFieldName).style.display="block";
                document.getElementById(field2Name).focus();
            document.getElementById(field2Name).style.backgroundColor="#f9ee8f";
            document.getElementById(field2Name).style.borderWidth="2px";
                                                return false;
                                }
                                else
                                {
            document.getElementById(errorFieldName).style.display="none";
            document.getElementById(field2Name).style.backgroundColor="#ffffff";
            document.getElementById(field2Name).style.borderWidth="1px";
                                    return true;
                                }
}

function validEmail(email)
{
                invalidchars = " /:;,"

                if(email == "")
                {                                              //checks to see if blank field
                                return false
                }
                                                
                for(i=0;i<invalidchars.length;i++){ //checks for invalid chars
                                                
                badchars = invalidchars.charAt(i)
                                                
                if(email.indexOf(badchars,0)!= -1)
                {
                                return false
                }              
                                                                                                                                                                                                                }
                
                atPos = email.indexOf("@",1)  //holds position of "@"
                                                
                if(atPos == -1) 
                {              //checks to see if "@" present
                                return false
                }
                                                                                                                
                if(email.indexOf("@",atPos+1) != -1) 
                { //checks for second "@"
                                return false
                }
                                                                                                                
                periodPos = email.indexOf(".",atPos) //holds position of "."
                                                
                if(periodPos == -1)
                { //checks for presence of "."
                                return false
                }
                if(periodPos+3 > email.length)
                {//makes sure at least two chars after the period
                                return false
                }
                return true 
}

function CheckedRadioValue(radioItem)
{
                for(i=0;i<radioItem.length;i++)
                {
                                if(radioItem[i].checked)
                                {
                                                return radioItem[i].value;
                                }//end if
                }//end for
                return false;
}

function CheckDemos()
{
    //Check Demographic radio buttons, check boxes, and dropdowns for required user input
    if (!myCheckRadioDemos(document.qualform2.JOB_FUNCTION, 'pJobFunctionError', 'JOB_FUNCTION_FIELDSET'))
    {   
        return false;
    }
    else if (!myCheckOtherDemos(document.qualform2.JOB_FUNCTION, '09', 'JOB_FUNCTION_DESC', 'pJobFunctionOtherError', 'JOB_FUNCTION_FIELDSET'))
    {
        return false;      
    }     
    else if (!myCheckRadioDemos(document.qualform2.PRIMARY_BUSINESS, 'pPrimaryBusinessError', 'PRIMARY_BUSINESS_FIELDSET'))
    {   
        return false;
    }   
    else if (!myCheckOtherDemos(document.qualform2.PRIMARY_BUSINESS, '12', 'PRIMARY_BUSINESS_DESC', 'pPrimaryBusinessOtherError', 'PRIMARY_BUSINESS_FIELDSET'))
    {
        return false;      
    }      
    else if (!myCheckCheckBoxDemos('PRODUCTS', 'pProductsError', 'PRODUCTS_FIELDSET'))
    {   
        return false;
    }           
    else if (!myCheckFormElementISNumeric('PERSONAL_ID', 'pPersonalIdentifierError', ''))
    {   
		return false;
    }   
    else
    {
        return true;
    }  
}

function myCheckRadioDemos(selectionFieldName, errorFieldName, selectionFieldSetName)
{
	if (!CheckUnselects(selectionFieldName)) 
	{ 
	    document.getElementById(selectionFieldSetName).scrollIntoView(true); 
        document.getElementById(selectionFieldSetName).className="standOutBcgrError";
        document.getElementById(errorFieldName).style.display="block";
	    document.getElementById(errorFieldName).focus();
	    return false;
	}
	else
	{
        document.getElementById(selectionFieldSetName).className="standOutBcgr grayGradient";
        document.getElementById(errorFieldName).style.display="none";
	    return true;
	}
}

function myCheckCheckBoxDemos(selectionFieldName, errorFieldName, selectionFieldSetName)
{
    if (!CheckboxChecked(document.qualform2, selectionFieldName)) 
    { 
	    document.getElementById(selectionFieldSetName).scrollIntoView(true); 
        document.getElementById(selectionFieldSetName).className="standOutBcgrError";
        document.getElementById(errorFieldName).style.display="block";
	    document.getElementById(errorFieldName).focus();
	    return false;
	}
	else
	{
        document.getElementById(selectionFieldSetName).className="standOutBcgr grayGradient";
        document.getElementById(errorFieldName).style.display="none";
	    return true;
	}
}

function myDropDownDemos(selectionFieldName, errorFieldName, selectionFieldSetName)
{
	if (!CheckDropdown(document.getElementById(selectionFieldName))) 
	{ 
	    document.getElementById(selectionFieldSetName).scrollIntoView(true); 
        document.getElementById(selectionFieldSetName).className="standOutBcgrError";
        document.getElementById(errorFieldName).style.display="block";
	    document.getElementById(errorFieldName).focus();
	    return false;
	}
	else
	{
        document.getElementById(selectionFieldSetName).className="standOutBcgr grayGradient";
        document.getElementById(errorFieldName).style.display="none";
	    return true;
	}
}

function myCheckOtherDemos(selectionFieldName, selectionFieldValue, textFieldName, errorFieldName, selectionFieldSetName)
{
    if (CheckedRadioValue(selectionFieldName) == selectionFieldValue && document.getElementById(textFieldName).value == '')	{ 
	    document.getElementById(selectionFieldSetName).scrollIntoView(true); 
        document.getElementById(selectionFieldSetName).className="standOutBcgrError";
        document.getElementById(errorFieldName).style.display="block";
	    document.getElementById(errorFieldName).focus();
	    return false;
	}
	else
	{
        document.getElementById(selectionFieldSetName).className="standOutBcgr grayGradient";
        document.getElementById(errorFieldName).style.display="none";
	    return true;
	}
}

function myCheckOtherChecked(selectionFieldName, selectionFieldValue, textFieldName, errorFieldName, selectionFieldSetName) {
    if ((CheckboxChecked2(document.qualform2, selectionFieldName)) && (document.getElementById(textFieldName).value == '')) {
        document.getElementById(selectionFieldSetName).scrollIntoView(true);
        document.getElementById(selectionFieldSetName).className = "standOutBcgrError";
        document.getElementById(errorFieldName).style.display = "block";
        document.getElementById(errorFieldName).focus();
        return false;
    }
    else {
        document.getElementById(selectionFieldSetName).className = "standOutBcgr grayGradient";
        document.getElementById(errorFieldName).style.display = "none";
        return true;
    }
}

function CheckDropdown(selectBox)
{

	if(selectBox.selectedIndex == "0")
	{
		return false;
	}
	else
	{
		return true;
	}
}

function CheckUnselects(radioItem)
{
	for(i=0;i<radioItem.length;i++)
	{
		if(radioItem[i].checked)
		{
			return true;
		}//end if
	}//end for
	return false;
}

function CheckboxChecked(thisform,CheckBoxName)
{
var ChLength = CheckBoxName.length + 1;
var vName = CheckBoxName + "-";

var CBindex = 0;

	for ( i=0; i < thisform.elements.length; i++)
	{
		if (thisform.elements[i].type == "checkbox")
		{
			if (thisform.elements[i].name.substring(0,ChLength)==vName)
			{
				if (CBindex==0) {CBindex = i;}
				if (thisform.elements[i].checked)
				{
					return true;
					break;
				}
			}
		}

	}

	thisform.elements[CBindex].focus();
	return false;
}


function CheckboxChecked2(thisform,CheckBoxName)
{
var ChLength = CheckBoxName.length + 1;
var vName = CheckBoxName;

var CBindex = 0;

                for ( i=0; i < thisform.elements.length; i++)
                {
                                if (thisform.elements[i].type == "checkbox")
                                {
                                                if (thisform.elements[i].name.substring(0,ChLength)==vName)
                                                {
                                                                if (CBindex==0) {CBindex = i;}
                                                                if (thisform.elements[i].checked)
                                                                {
                                                                                return true;
                                                                                break;
                                                                }
                                                }
                                }

                }

                thisform.elements[CBindex].focus();
                return false;
}



