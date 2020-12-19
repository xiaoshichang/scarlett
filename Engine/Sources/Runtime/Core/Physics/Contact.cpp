#include "Contact.h"

void scarlett::ContactManifold::UpdateContacts()
{


}

void scarlett::ContactManifold::AddContact(ContactPoint point)
{
	if (contactPointCount >= 4)
	{
		return;
	}

	contactPointCount += 1;
	contactPoints[contactPointCount] = point;
}
