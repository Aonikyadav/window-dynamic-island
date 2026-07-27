import express from 'express';
import cors from 'cors';
import { MongoClient } from 'mongodb';
import dotenv from 'dotenv';

dotenv.config();

const app = express();
const PORT = process.env.PORT || 5000;
const MONGODB_URI = process.env.MONGODB_URI || 'mongodb+srv://aonikyadavcs23_db_user:Eventra-CampuTix@cluster0.nbrl6dn.mongodb.net/test?retryWrites=true&w=majority&appName=Cluster0';
const DB_NAME = process.env.DB_NAME || 'test';
const COLLECTION_NAME = process.env.COLLECTION_NAME || 'users';

app.use(cors());
app.use(express.json());

let dbClient = null;

async function getCollection() {
  if (!dbClient) {
    dbClient = new MongoClient(MONGODB_URI);
    await dbClient.connect();
    console.log('Connected successfully to Cluster0 -> test -> users');
  }
  return dbClient.db(DB_NAME).collection(COLLECTION_NAME);
}

// POST /api/register - Save user details into "users" collection in "test" database (Eventra Project)
app.post('/api/register', async (req, res) => {
  try {
    const { fullName, email, profession, collegeName, yearOfStudy } = req.body;

    if (!fullName || !email || !profession) {
      return res.status(400).json({ success: false, message: 'Full name, email, and profession are required.' });
    }

    if (profession === 'Student' && (!collegeName || !yearOfStudy)) {
      return res.status(400).json({ success: false, message: 'College name and year of study are required for students.' });
    }

    const collection = await getCollection();

    const userDoc = {
      name: fullName.trim(),
      fullName: fullName.trim(),
      email: email.trim().toLowerCase(),
      profession,
      collegeName: profession === 'Student' ? collegeName.trim() : null,
      yearOfStudy: profession === 'Student' ? yearOfStudy : null,
      source: 'Dynamic-Island-for-Windows',
      app: 'Dynamic-Island-for-Windows',
      createdAt: new Date(),
    };

    const result = await collection.insertOne(userDoc);
    console.log(`Saved new user to "users" collection in database "test" (Eventra Project). ID: ${result.insertedId}`);

    return res.status(200).json({
      success: true,
      message: 'Registration saved successfully in Eventra MongoDB (test -> users).',
      id: result.insertedId,
    });
  } catch (error) {
    console.error('Error saving to Eventra MongoDB:', error);
    return res.status(500).json({ success: false, message: 'Failed to save registration to Eventra MongoDB database.', error: error.message });
  }
});

app.get('/health', (req, res) => {
  res.json({ status: 'OK', company: 'Eventra (CampuTix)', database: 'test', collection: 'users' });
});

app.listen(PORT, () => {
  console.log(`Eventra Dynamic Island MongoDB Server running on http://localhost:${PORT}`);
});
