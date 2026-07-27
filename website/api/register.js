import { MongoClient } from 'mongodb';

const MONGODB_URI = process.env.MONGODB_URI || 'mongodb+srv://aonikyadavcs23_db_user:Eventra-CampuTix@cluster0.nbrl6dn.mongodb.net/Eventra?retryWrites=true&w=majority&appName=Cluster0';
const DB_NAME = process.env.DB_NAME || 'Eventra';
const COLLECTION_NAME = 'Dynamic Island Users';

let cachedClient = null;

async function connectToDatabase() {
  if (cachedClient) return cachedClient;
  const client = new MongoClient(MONGODB_URI);
  await client.connect();
  cachedClient = client;
  return client;
}

export default async function handler(req, res) {
  if (req.method !== 'POST') {
    return res.status(405).json({ success: false, message: 'Method Not Allowed' });
  }

  try {
    const { fullName, email, profession, collegeName, yearOfStudy } = req.body;

    if (!fullName || !email || !profession) {
      return res.status(400).json({ success: false, message: 'Full name, email, and profession are required.' });
    }

    if (profession === 'Student' && (!collegeName || !yearOfStudy)) {
      return res.status(400).json({ success: false, message: 'College name and year of study are required for students.' });
    }

    const client = await connectToDatabase();
    const db = client.db(DB_NAME);
    const collection = db.collection(COLLECTION_NAME);

    const userDoc = {
      fullName: fullName.trim(),
      email: email.trim().toLowerCase(),
      profession,
      collegeName: profession === 'Student' ? collegeName.trim() : null,
      yearOfStudy: profession === 'Student' ? yearOfStudy : null,
      company: 'Eventra',
      app: 'Dynamic-Island-for-Windows',
      createdAt: new Date(),
    };

    const result = await collection.insertOne(userDoc);

    return res.status(200).json({
      success: true,
      message: 'Registration saved successfully in Eventra MongoDB.',
      id: result.insertedId,
    });
  } catch (error) {
    console.error('Serverless Eventra MongoDB Error:', error);
    return res.status(500).json({ success: false, message: 'Database insertion error', error: error.message });
  }
}
